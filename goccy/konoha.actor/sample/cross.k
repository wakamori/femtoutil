using konoha.actor.*;

/***
* $ scalac sample/ReceiveActor.scala
* $ scala sample/ReceiveActor
* $ scalac ScalaActor.scala
* $ konoha sample/cross.k (this script) ===<send 54>===> ReceiveActor
***/

Actor.sendToScalaActor("scala_actor", "127.0.0.1", 5454, 54);
