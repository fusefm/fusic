/*
    fusic - radio playout software.
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

#include "authldap.h"

#include <ldap.h>
#include <QSettings>

void authLDAP::doAuth()
{
  LDAP* handle;
  
  // Attempt connect.
  int res = ldap_initialize(&handle, mLDAPURI.toLatin1().data());
  if(res != LDAP_SUCCESS)
  {
    emit authError("Could not init the LDAP library: " + getLDAPErrorString(res));
    return;
  }
  
  // Set timeout.
  res = ldap_set_option(handle, LDAP_OPT_TIMEOUT, &mTimeout);
  if(res != LDAP_OPT_SUCCESS)
  {
    emit authError("Could not set LDAP timeout: " + getLDAPErrorString(res));
    return;
  }
  
  // Build the bind DN.
  QString bindDN = mBindAttribute;
  bindDN += "=";
  bindDN += mUserID;
  bindDN += ",";
  bindDN += mBaseDN;
  
  // Construct a berval for the password.
  struct berval* pass = NULL;
  pass = (struct berval*)malloc(sizeof (struct berval));
  pass->bv_len = mPassword.length();
  pass->bv_val = mPassword.toLatin1().data();
  
  res = ldap_sasl_bind_s(handle, bindDN.toLatin1().data(), NULL, pass, NULL, NULL, NULL);
  
  // Cleanup mem.
  free(pass);
  
  if(res == LDAP_INVALID_CREDENTIALS)
  {
    ldap_unbind_ext_s(handle, NULL, NULL);
    emit finishedAuth(false);
    return;
  }
  if(res != LDAP_SUCCESS)
  {
    ldap_unbind_ext(handle, NULL, NULL);
    emit authError("Could not bind: " + getLDAPErrorString(res));
    return;
  }
  
  // Search through the access groups.
  bool foundUserInAccessGroup = false;
  foreach(QString accessGroup, mAccessGroups)
  {
    LDAPMessage* resChain = NULL;  //to get LDAP results.
    LDAPMessage* resOne = NULL;    //the individual result.
    struct berval **val;           //for storing an array of users for an LDAP group.
    
    // Build the filter string.
    QString searchString;
    searchString = "(cn=";
    searchString += accessGroup;
    searchString += ")";
    
    // Perform the search for the group.
    res = ldap_search_ext_s(handle, mBaseDN.toLatin1().data(), LDAP_SCOPE_SUBTREE,
      searchString.toLatin1().data(), NULL, 0, NULL, NULL, NULL, 0, &resChain);
    
    if(res != LDAP_SUCCESS || resChain == NULL)
    {
      ldap_unbind_ext(handle, NULL, NULL);
      emit authError("Could not perform LDAP subtree search: " + getLDAPErrorString(res));
      return;
    }
    
    // Get the first result.
    resOne = ldap_first_entry(handle, resChain);
    if(resOne == NULL)
    {
      ldap_unbind_ext(handle, NULL, NULL);
      emit authError("Got NULL resOne.");
      return;
    }
    
    //get all "memberUid" attributes (an array of user ids
    //that are in the group):
    val = ldap_get_values_len(handle, resOne, "memberUid");
    if(val == NULL)
    {
      ldap_unbind_ext(handle, NULL, NULL);
      emit authError("Got NULL val.");
      return;
    }
    
    // Count the number of users;
    int noOfUsers = ldap_count_values_len(val);
    
    for(int i = 0; i < noOfUsers; i++)
    {
      const char* userID = mUserID.toLatin1().data();
      if(strcmp(userID, val[i]->bv_val) == 0)
      {
	// We have found the user in an access group.
	foundUserInAccessGroup = true;
	
	ldap_value_free_len(val);
	ldap_msgfree(resChain);
	break;
      }
    }
    
    // We don't need to continue searching through groups if we've already found one.
    if(foundUserInAccessGroup)
      break;
    
    // Free membory for next search.
    ldap_value_free_len(val);
    ldap_msgfree(resChain);
    res = NULL;
    resChain = NULL;
    val = NULL;
  }
  
  ldap_unbind_ext(handle, NULL, NULL);
  
  if(foundUserInAccessGroup)
    emit finishedAuth(true);
  else
    emit finishedAuth(false);
}

const QDialog* authLDAP::getsettingsDialog(QWidget* parent)
{
  return authBase::getsettingsDialog(parent);
}

const QString authLDAP::getLDAPErrorString(int errnumber)
{
  return QString(ldap_err2string(errnumber));
}

bool authLDAP::getLdapSettings()
{
  QSettings set;
  int timeout;
  
  // Connection URI.
  mLDAPURI = set.value("LDAPSettings/URI").toString();
  
  // Bind attribute.
  mBindAttribute = set.value("LDAPSettings/BindAttribute").toString();
  
  // Base DN.
  mBaseDN = set.value("LDAPSettings/BaseDN").toString();
  
  // Access groups.
  mAccessGroups = set.value("LDAPSettings/AccessGroups").toStringList();
  
  // Timeout.
  timeout = set.value("LDAPSettings/Timeout", -1).toInt();
  mTimeout.tv_sec = timeout;
  mTimeout.tv_usec = 0;
  
  // Check settings.
  if(mLDAPURI.isEmpty() || mBindAttribute.isEmpty() || mBaseDN.isEmpty())
    return false;
  else
    return true;
}
