#!/usr/local/bin/konoha

using konoha.xml.*;


class BPMN2Node {
	Map<String, String> attributes;

	void printKeys () {
		String[] keys = attributes.keys();
		foreach(String key in keys) {
			OUT <<< key <<< "," <<<  attributes[key] <<< EOL;
		}
	}

}


/* BPMN2Node */
class DefinitionsNode extends BPMN2Node {
	DefinitionsNode () {
		attributes = {};
	}

}

class ParticipantNode extends BPMN2Node {
	ParticipantNode() {
		attributes = {};
	}
}

class CollaborationNode extends BPMN2Node {
	CollaborationNode () {
		attributes = {};
	}
}

class ProcessNode extends BPMN2Node {
	ProcessNode () {
		attributes = {};
	} 
}

class StartEventNode extends BPMN2Node {
	StartEventNode () {
		attributes = {};
	}
}

class EndEventNode extends BPMN2Node {
	EndEventNode () {
		attributes = {};
	}
}

class ScriptTaskNode extends BPMN2Node {
	ScriptTaskNode () {
		attributes = {};
	}
}


class BPMN2Edge {
	String id;
}
class IncomoingEdge extends BPMN2Edge {
	IncomoingEdge () {
		id = "";
	}
}

class OutgoingEdge extends BPMN2Edge {
	OutgoingEdge () {
		id = "";
	}
}
class TaskNode extends BPMN2Node {
	BPMN2Edge[] edges;

	TaskNode () {
		attributes = {};
		edges = [];
	}
}


/* BPMNDiagramNode */

class BPMNDiagramNode {
	Map<String, String> attributes;
}

class DCElement {
	Map<String, String> attributes;
}

class BoundsElement extends DCElement {
	BoundsElement () {
		attributes = {};
	}
}

class DiagramNode extends BPMNDiagramNode {
	DiagramNode () {
		attributes = {};
	}
}

class PlaneNode extends BPMNDiagramNode {
	PlaneNode () {
		attributes = {};
	}
}

class ShapeNode extends BPMNDiagramNode {
	ShapeNode () {
		attributes = {};
	}
}

class DReader {
	XmlReader reader;
	String filename;

	Array<BPMN2Node> head;
	Array<BPMNDiagramNode> dhead;

	DReader (String xmlfile) {
		filename = xmlfile;
		reader = new XmlReader(filename);
		head = [];
		dhead = [];
	}

	void processNode (BPMN2Node node) {
		int attrCount = 0;
		if (reader.hasAttributes()) {
			reader.constName();
			attrCount = reader.getAttributeCount();
		} else {
			print "no attribute! at " + reader.constName();
			return;
		}
		for (i = 0; i < attrCount; i++) {
			reader.moveToNextAttribute();
			if (reader.nodeType() == XmlReader.TYPE_ATTRIBUTE) {
				node.attributes[reader.constName()] = reader.constValue();
			} else {
				print "error!";
				break;
			}
		}
	}

	void doProcess () {
		while(reader.read()){
			type = reader.nodeType();
			String nodeType = reader.constName();
			BPMN2Node node = null;
			BPMNDiagramNode dnode;
			Boolean isEdge = false;
			if (nodeType.startsWith("bpmn2") == true) {
				switch (nodeType) {
				case "bpmn2:definitions":
					node = new DefinitionsNode();
					break;
				case "bpmn2:collaboration":
					node = new CollaborationNode();
					break;
				case "bpmn2:participant":
					node = new ParticipantNode();
					break;
				case "bpmn2:process":
					node = new ProcessNode();
					break;
				case "bpmn2:task": case "bpmn2:scriptTask":
				case "bpmn2:userTask":
					node = new TaskNode();
					break;
				case "bpmn2:incoming":
					isEdge = true;
					break;
				case "bpmn2:outgoing":
					isEdge = true;
					break;
				case "bpmn2:sequenceFlow":
					isEdge = true;
					break;
				case "bpmn2:startEvent":
					node = new StartEventNode();
					break;
				case "bpmn2:endEvent":
					node = new EndEventNode();
					break;
				default:
					print nodeType;
				}
				if (isEdge != true && node != null) {
					processNode(node);
					head.add(node);
				}
			} else {
				switch (nodeType) {
				case "bpmndi:BPMNEdge":
					break;
				case "di:waypoint":
					break;
				case "bpmndi:BPMNPlane":
					break;
				case "bpmndi:BPMNDiagram":
					break;
				case "bpmndi:BPMNShape":
					break;
				case "dc:Bounds":
					break;
				default:
					print nodeType;
				}

			}
		} /* while */
	} 

	void showNodeTree () {
		int len = |head|;
		for(int i = 0; i < len; i++) {
			BPMN2Node node = head[i];
			OUT <<< "[" <<< node.class <<< "] ";
			node.printKeys();
		}
	}
}

int main(String[] args)
{
	DReader dr;
	int argc = |args|;
	print argc;
	if (argc < 1) {
		OUT.println("usage: konoha parser.k $(filename)");
		return 0;
	}
	print args[0];
	String filename = args[0];
	dr = new DReader(filename);
	dr.doProcess();
	dr.showNodeTree();
	
	return 0;
}
