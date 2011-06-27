import scala.actors._
import scala.actors.Actor._
import Console._
import scala.actors.Exit

object Ring {
	def main(args: Array[String]) = {
		val begin = System.nanoTime()
			start(args(0).toInt, args(1).toInt, args(2))
			val end = System.nanoTime()
			// println(end - begin)
		;
	}

	def makeActor(last:Boolean):Actor = {
		actor {
			react {
				case neighbour:Actor =>
					       //println("I'm [" + self + "], my neighbour is [" + neighbour + "].");
					       loop {
						       react {
							       case (msg:Any, count:Int) => 
									 //println("I'm [" + self + "], received a message: " + msg.toString() + "/" + count);
									 neighbour ! (msg, count - (if (last) 1 else 0));
									 //if (count == 1) exit()
									 if (count == 1) exit(if (last) 'notify_end else 'normal)
						       }
					       }
			}
		}
	}
	def start(n:Int, m:Int, msg:String) = {
		val actors @ (h::t) = for (i <- List.range(0,n)) yield makeActor(i == n - 1);
		val neighbours = t ::: List(h)
			for ((actor, neighbour) <- actors.zip(neighbours)) actor ! neighbour;
		self.trapExit = true
			link(actors.last)
			h ! (msg, m);
		//receive { case ('EXIT, _, _) => () }
		receive { case Exit(_, 'notify_end) => () }
	}
}
