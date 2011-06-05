#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
AspenStorage:
  for storing user data
  shinpei(c)2011
"""

from sqlite3 import dbapi2 as sqlite
import uuid, time

"""
globals
"""

db_name = "aspen.sqlite3"
table_name = "aspen"
personal_tbl_name = "personal"
session_tbl_name = "sessions"


"""
 class AspenSession
"""

class AspenSession:
    def __init__(self, uid, passwd):
        self.__uid = uid;
        self.__passwd = passwd;

    def generateSID(self ):
        self.__sid = uuid.uuid4();

    def getSID (self):
        return self.__sid;

    def getUID(self):
        return self.__uid;

    def getPasswd(self):
        return self.__passwd;

"""
class AspenStorage
"""

class AspenStorage:
    def __init__(self):
        self.dbname = db_name;
        self.con = sqlite.connect(db_name)
        self.cur = self.con.cursor()

    def generateSession(self):
        return AspenSession();

    def authenticate (self, session):
        uid = session.getUID();
        self.cur.execute('select passwd from personal where uid="' + uid + '";')
        passwd = ""
        for row in self.cur:
            passwd = row[0]
        if session.getPasswd() == passwd:
            session.generateSID();
            return True;
        return False;

    def commitSession (self, session):
        if session.getSID() != "" :
            query = "insert into " + session_tbl_name + " values(";
            query = query + '"' + str(session.getSID()) + '", ';
            query = query + '"' + session.getUID() + '", ';
            query = query + '"' + str(time.mktime(time.localtime())) + '", ';
            query = query + '"Untitled");';
            print query;
            self.cur.execute(query)
            self.con.commit()
    def close(self):
        self.con.close();
        

    # name SID 
    #  assume that sid is already exists!
    #  if its not exist, error.
    def nameSID(self, session, filename):
        if session.getSID() != "":
            query = "update " + session_tbl_name + " set ";
            query = query + 'filename="' + filename + '" where sid="';
            query = query + str(session.getSID()) + '";';
            self.cur.execute(query);
            self.con.commit();


    # sid --> filename
    def sid2filename(self, sid):
        return;



if __name__ == '__main__':
    ast = AspenStorage();
    ses = AspenSession("d11gd186", "konoha");
    if ast.authenticate(ses) == True:
        ast.commitSession(ses);
        ast.nameSID(ses, "Hello world");
        print "authenticate done!"
    else :
        print "authentication failed"
    ast.close()
