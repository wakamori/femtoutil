#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
AspenStorage:
  for storing user data
  shinpei(c)2011
"""

from sqlite3 import dbapi2 as sqlite
import uuid, time
import sys
import random

"""
globals
"""

db_name = "../db/aspen.sqlite3"
table_name = "aspen"
personal_tbl_name = "personal"
session_tbl_name = "sessions"
flow_tbl_name = "flows"
user_tbl_name = "users"

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
            self.commitSession(session);
            return True;
        return False;

    # ======== added by wakamori ========
    def hasUser(self, uid):
        query = "select * from %s where uid=?;" % user_tbl_name
        self.cur.execute(query, (uid,))
        row = self.cur.fetchone()

        if row == None:
            return False

        return True

    def selectRecord(self, record, sid):
        query = "select %s from %s where sid=?;" % (record, user_tbl_name)
        self.cur.execute(query, (sid,))
        row = self.cur.fetchone()

        if row == None:
            # no user has specified session
            return None

        return row[0]

    def getUID(self, sid):
        return self.selectRecord('uid', sid)

    def getScreenName(self, sid):
        return self.selectRecord('screen_name', sid)

    def renewSession(self, uid):
        query = "update %s set sid=? where uid=?;" % user_tbl_name
        asession = AspenSession(uid, '')
        asession.generateSID()
        sid = str(asession.getSID())
        self.cur.execute(query, (sid, uid))
        self.con.commit()
        return sid

    def getAccessToken(self, sid):
        return [self.selectRecord('access_token', sid),
            self.selectRecord('access_token_secret', sid)]

    def createUserWithToken(self, uid, screen_name, access_token,
            access_token_secret):
        query = "insert into %s values(?,?,?,?,?);" % user_tbl_name

        try:
            self.cur.execute(query, (uid, '', screen_name, access_token,
                access_token_secret))
            self.con.commit()
        except sqlite.IntegrityError, err:
            sys.stderr.write("ERROR: %s\n" % str(err))
            sys.stderr.write("'%s' is probably already exests in database.\n" %
                    (uid))
            return False

        return True
    # ===================================

    # authenticate with a pair of uid, and sid
    def authenticateWithUSID (self, uid, sid):
        # validate uid, sid;
        query = 'select uid from ' + session_tbl_name;
        query = query + ' where sid="' + sid + '";';
        self.cur.execute(query);
        for row in self.cur:
            luid = str(row[0]);
            if luid == uid:
                retSession = AspenSession(uid, "");
                return retSession;
        return None;

    def authenticateWithUSIDAndRenewSession (self, uid, sid):
        # validate uid, sid;
        query = 'select uid from ' + session_tbl_name;
        query = query + ' where sid="' + sid + '";';
        self.cur.execute(query);
        for row in self.cur:
            luid = str(row[0]);
            if luid == uid:
                retSession = AspenSession(uid, "");
                retSession.generateSID();
                self.commitSession(retSession);
                return retSession;
        return None;

    def addUserRetPasswd(self, uid):
        s = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890';
        passwd = "";
        for i in range(8):
            passwd += random.choice(s)
        query = "insert into " + personal_tbl_name + ' values("' + uid + '","", "' + passwd + '");';
        try:
            self.cur.execute(query);
            self.con.commit();
        except sqlite.IntegrityError, err:
            sys.stderr.write("ERROR: %s\n" % str(err))
            sys.stderr.write("'%s' is probably already exists in database.\n" % (uid))
            return "none"
        return passwd;

    # commitSession is write session into the Database
    # it invoked after the authentication.
    # this method is internal use only.
    def commitSession (self, session):
        if session.getSID() != "" :
            query = "insert into " + session_tbl_name + " values(";
            query = query + '"' + str(session.getSID()) + '", ';
            query = query + '"' + session.getUID() + '", ';
            query = query + '"' + str(time.mktime(time.localtime())) + '", ';
            query = query + '"Untitled");';
            # print query;
            self.cur.execute(query);
            self.con.commit();
            
            ## and, record it to flow
            sid = self.getLatestSID(session.getUID());
            query = "insert into " + flow_tbl_name + " (uid, from_sid, to_sid) values(";
            query = query + '"' + str(session.getUID()) + '", "';
            query = query + sid + '", "';
            query = query + str(session.getSID()) + '");';
            self.cur.execute(query);
            self.con.commit();

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
        filename = "";
        query = 'select filename where sid="';
        query = query + str(sid) + '";';
        self.cur.execute(query);
        for row in self.cur:
            filename = row[0]
        return filename;

    # get current link and rewind them
    def rewindSID(self, sid):
        fromsid = "";
        query = 'select from_sid from ' + flow_tbl_name + ' where to_sid="';
        query = query + str(sid) + '";';
        self.cur.execute(query);
        for row in self.cur:
            if row is not None:
                fromsid = row[0];
            else:
                # in case there is no rewind. its a root!
                fromsid = "";
        return fromsid;

    def forwardSID(self, sid):
        tosid = "";
        query = 'select to_sid from ' + flow_tbl_name + ' where from_sid="';
        query = query + str(sid) + '";';
        self.cur.execute(query);
        for row in self.cur:
            if row is not None:
                tosid = row[0];
            else:
                tosid = "";
        return tosid;


    def getLatestSID (self, uid):
        latestSID = "";
        query = 'select id, to_sid from ' + flow_tbl_name + ' where uid="';
        query = query + str(uid) + '";';
        self.cur.execute(query);
        rets = [];
        for row in self.cur:
            rets.append([row[0], row[1]]);
        if rets == []:
            return "none";
        rets.sort();
        return rets.pop()[1];


    # TODO: the func below wont work.
    def getFilesOfUser(self, uid):
        query = "select sid, filename from " + session_tbl_name;
        query = query + ' where uid="' + uid + '";';
        self.cur.execute(query);
        ret = [];
        for row in self.cur:
            ret.add(row[0],row[1]);
        return ret;



    # show methods
    #  - these methods are only printing information.
    #  - used only for debug use
 
    # show users
    def showAllUsers(self):
        query = "select uid, passwd from " + personal_tbl_name;
        self.cur.execute(query);
        for row in self.cur:
            print row[0] + "," + row[1]
    
    # show files for a single user
    def showFilesOfUser(self, uid):
        query = "select sid, filename from " + session_tbl_name;
        query = query + ' where uid="' + uid + '";';
        self.cur.execute(query);
        for row in self.cur:
            print row[0] + "," + row[1]



if __name__ == '__main__':
    ast = AspenStorage();
    ses = AspenSession("aspen", "konoha");
    if ast.authenticate(ses):
        sid = ast.getLatestSID(ses.getUID());
        print sid;
        rsid = ast.rewindSID(sid);
        fsid = ast.forwardSID(rsid);
        print fsid;
    ast.close()
