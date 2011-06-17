import scala.actors.Actor._
import scala.actors.remote.Node
import scala.actors.remote.RemoteActor._

object ScalaActor extends Application {

	def sendInt(target: String, host: String, port: Int, args: Int) {
		actor {
		  //println("hello scala world");
		  //println(host);
		  //println(port);
		  //println(args);
		  val consumer = select(Node(host, port), Symbol(target))
          consumer !? args match {
            case msg => println("result=" + msg)
          }
		}
	}
}
