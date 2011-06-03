#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
AspenStorage:
  for storing user datas

"""

from sqlite3 import dbapi2 as sqlite
import uuid

"""
globals
"""

db_name = "aspen.sqlite3"
table_name = "aspen"

"""
 class AspenSession
"""

class AspenSession:
    def __init__(self, uid, passwd):
        self.uid = uid;
        self.passwd = passwd;

    def generateSID(self ):
        self.__sid = uuid.uuid4();


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
        uid = session.uid;
        self.cur.execute('select passwd from personal where uid="' + uid + '";')
        passwd = ""
        for row in self.cur:
            passwd = row[0]
        if session.passwd == passwd:
            session.generateSID();
            return True;
        return False;

    

        
if __name__ == '__main__':
    ast = AspenStorage();
    ses = AspenSession("d11gd186", "konoha");
    if ast.authenticate(ses) == True:
        print "authenticate done!"
