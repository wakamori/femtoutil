import scala.actors.Actor._
import scala.actors.remote.RemoteActor._

object ReceiveActor extends Application {
    actor {
        alive(5454)
        register('scala_actor, self)
        loop {
            receive {
                case msg => {
                    println("received=" + msg)
                    reply("rec-" + msg)
                }
            }
        }
    }
}
