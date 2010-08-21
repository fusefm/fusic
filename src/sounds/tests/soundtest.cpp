/*
    fusic - Radio playout software
    Copyright (C) 2010  Matthew Leach

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "soundtest.h"
#include <fusicsettings.h>
#include <song.h>
#include <QTest>
#include <QDebug>
#include <QSignalSpy>

void soundTest::initSettings()
{
  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
  db.setDatabaseName(QString("fusic_tests"));
  db.setHostName("localhost");
  db.setUserName("root");
  db.setPassword("system12");
}

void soundTest::initFakeSettings()
{
  fusicSettings::DBSettings::setDatabase(QString("fooTable"));
  fusicSettings::DBSettings::setHost(QString("fooHost"));
  fusicSettings::DBSettings::setPassword(QString("fooPassword"));
  fusicSettings::DBSettings::setUserName(QString("fooUser"));
}


void soundTest::testSoundSetup()
{
  // Initalise the settings.
  initSettings();
  
  // Create the object.
  Song theSound(1);
  
  // Ensure a valid object.
  QVERIFY(theSound.isValid());
  
  // Test the data.
  QCOMPARE(theSound.getTitle(),QString("Some Song"));
  QCOMPARE(theSound.getLocation(), QString("/home/user/music/smomeSound.mp3"));
  QCOMPARE(theSound.getDuration(), 33.886);
}

void soundTest::testSoundAsyncSetup()
{
  
  Sound theSound;
  
  QSignalSpy spy(&theSound, SIGNAL(setupComplete(bool)));
  theSound.setFileID(1);
  QTest::qWait(1000);
  QVERIFY(spy.count());
  QVERIFY(theSound.isValid());
  
  // Test the data.
  QCOMPARE(theSound.getTitle(),QString("Some Song"));
  QCOMPARE(theSound.getLocation(), QString("/home/user/music/smomeSound.mp3"));
  QCOMPARE(theSound.getDuration(), 33.886);
  
}
void soundTest::testSoundInvalidation()
{
  // Kill the database.
 QSqlDatabase db = QSqlDatabase::database();
 db.close();
 db.setHostName("foo");
 
  Sound theSound;
  Sound anotherSound(1);
  // Ensure the object is not valid.
  QVERIFY(!theSound.isValid());
  
  QVERIFY(!anotherSound.isValid());
}

QTEST_MAIN(soundTest);
#include "soundtest.moc"