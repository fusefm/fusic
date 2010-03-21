-- MySQL Administrator dump 1.4
--
-- ------------------------------------------------------
-- Server version	5.0.51a-24+lenny2-log


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


--
-- Create schema fusic
--

CREATE DATABASE IF NOT EXISTS fusic;
USE fusic;
CREATE TABLE `broadcasts` (
  `broadcast_ID` int(11) NOT NULL auto_increment,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY  (`broadcast_ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `carts` (
  `sound_ID` int(10) unsigned NOT NULL,
  `row` tinyint(3) unsigned NOT NULL COMMENT 'Position on the screen X',
  `col` tinyint(3) unsigned NOT NULL COMMENT 'Position on the screen Y',
  `page` tinyint(3) unsigned NOT NULL COMMENT 'Page number on screen',
  `colour` varchar(6) NOT NULL COMMENT 'Colour in hex format (e.g. FFFFFF)',
  `playmode` tinyint(3) unsigned NOT NULL COMMENT 'The number of the playmode (see software)'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Extra info used by cart sounds';
CREATE TABLE `log` (
  `log_ID` int(10) unsigned NOT NULL auto_increment,
  `sound_ID` int(10) unsigned NOT NULL,
  `show_ID` int(10) unsigned NOT NULL,
  `date_time` datetime NOT NULL,
  PRIMARY KEY  (`log_ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `playlist_sounds` (
  `playlist_ID` int(10) unsigned NOT NULL,
  `sound_ID` int(10) unsigned NOT NULL,
  `position` int(11) NOT NULL COMMENT 'Number which is used to sort a playlist'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `playlists` (
  `playlist_ID` int(10) unsigned NOT NULL auto_increment,
  `show_ID` int(10) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY  (`playlist_ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `settings` (
  `key` varchar(255) NOT NULL,
  `value` blob NOT NULL,
  PRIMARY KEY  (`key`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `show_users` (
  `show_ID` int(10) unsigned NOT NULL,
  `username` varchar(255) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE `shows` (
  `show_ID` int(10) unsigned NOT NULL auto_increment,
  `name` varchar(255) NOT NULL,
  `broadcast_ID` int(10) unsigned NOT NULL COMMENT 'The broadcast ID',
  PRIMARY KEY  (`show_ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Radio shows (one extra word: ZOMG)';
CREATE TABLE `songs` (
  `sound_ID` int(10) unsigned NOT NULL,
  `artist` varchar(255) NOT NULL,
  `album` varchar(255) default NULL,
  `genre` varchar(255) default NULL,
  `intro` double unsigned NOT NULL COMMENT 'Time before vocals cut in on song (seconds)'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Extra info for song sounds';
CREATE TABLE `sounds` (
  `sound_ID` int(10) unsigned NOT NULL auto_increment,
  `type_ID` tinyint(3) unsigned NOT NULL COMMENT 'The type of sound',
  `show_ID` int(10) unsigned default NULL COMMENT 'The show that owns this sound (or NULL if global)',
  `location` varchar(100) NOT NULL COMMENT 'Filename of the song (SHA256 for the name and extension as normal)',
  `title` varchar(255) NOT NULL COMMENT 'Human-readable title for this file',
  `duration` double unsigned NOT NULL COMMENT 'Playtime in seconds',
  `deleted` tinyint(1) NOT NULL default '0' COMMENT 'Has the file been deleted',
  PRIMARY KEY  (`sound_ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='All sounds (e.g. songs, carts, jingles) known to the system';
CREATE TABLE `types` (
  `type_ID` tinyint(3) unsigned NOT NULL auto_increment,
  `description` varchar(100) NOT NULL COMMENT 'A human readable description',
  PRIMARY KEY  (`type_ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Type of a sound';



/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
