#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
AspenStorage:
  for storing user datas

"""

import sqlite3, uuid

"""
globals
"""

DB_NAME = "aspen.sqlite3"

"""
 class AspenSession
"""

class AspenSession:
    def __init__(uid, passwd):
        self.uid = uid;
        self.passwd = passwd;

    def generateSID():
        self.__sid = uuid.uuid4();


"""
class AspenStorage
"""

class AspenStorage:
    def __init__(self):
        self.dbname = DBNAME
        self.db = sqlite3.connect(DB_NAME, isolation_level=None)
        self.connection = self.db.cursor()


    def generateSession(self):
        return AspenSession();

        
