#!/usr/local/bin/konoha
/* -------------------------------------------
 * parser.k : 
 *  bpmn2 parser
 *  shinpei(c)2011
 *
 * ------------------------------------------- */

using konoha.xml.*;

include "bpmn2.k";
class BPMNFlow {
	TaskNode startevent;
	TaskNode endevent; // must be Array<TaskNode>
	Array<TaskNode> tasks;
	Array<SequenceFlowNode> flows;
	
	BPMNFlow () {
		tasks = [];
		flows = [];
	}

	String generateCode () {
		String code = '';
		int fnum = |flows|;
		int tnum = |tasks|;
		if (fnum == 0  || tnum == 0) return "";
		
		// task parse
		Map <String, TaskNode> map_task = {};
		foreach (TaskNode t in tasks) {
			t.taskname = t.attributes['id'];
			if (t.taskname != null) {
				print t.attributes['id'];
				map_task[t.taskname] = t;
			}
		}
		
		// assign source, target 
		String s_source;
		String s_target;
		Map<String, TaskNode> map_fromTo = {};

		//TODO:
		foreach (SequenceFlowNode s in flows) {
			String sr = s.attributes["sourceRef"];
			String tr = s.attributes["targetRef"];
			//print sr, tr;
			map_fromTo[sr] = map_task[tr];
		}
		

		// script generating
		// begin from start event
		String startEventName = startevent.attributes["id"];
		TaskNode t = startevent;
		TaskNode toNode;
		DTaskLoader dtl = new DtaskLoader("dtask");
		script = "";
		while (true) {
			if (t == endevent) break;
			toNode = map_fromTo[t.taskname];
			script += dtl.fetch(toNode.taskname);
			t = toNode;
		}

		script += """
int main(String[] args) {
""";
		t = startevent;
		while (true) {
			toNode = map_fromTo[t.taskname];
			t = toNode;
			if (t == endevent) break;
			script += "\t" + t.taskname + "();\n";
		}
		script += """
} /* end of main */
"""
		return script;
	}
}

class BPMNParser {
	XmlReader reader;
	String filename;

	Array<BPMN2Node> allNodes;
	Array<BPMNDiagramNode> dhead;

	BPMNParser (String xmlfile) {
		filename = xmlfile;
		reader = new XmlReader(filename);
		allNodes = [];
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
		for (int i = 0; i < attrCount; i++) {
			reader.moveToNextAttribute();
			if (reader.nodeType() == XmlReader.TYPE_ATTRIBUTE) {
				node.attributes[reader.constName()] = reader.constValue();
			} else {
				print "error!";
				break;
			}
		}
	}

//	void parse () {
	BPMNFlow genFlow () {
		BPMNFlow bflow = new BPMNFlow();
		int type;
		String nodeType;
		BPMN2Node node;
		BPMNDiagramNode dnode;
		boolean isEdge = false;
		while(reader.read()){
			type = reader.nodeType();
			nodeType = reader.constName();
			node = null;
			isEdge = false;

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
					bflow.tasks.add((TaskNode)node);
					break;
				case "bpmn2:incoming":
					isEdge = true;
					break;
				case "bpmn2:outgoing":
					isEdge = true;
					break;
				case "bpmn2:sequenceFlow":
					node = new SequenceFlowNode();
					bflow.flows.add((SequenceFlowNode)node);
					break;
				case "bpmn2:startEvent":
					// ASSURE there is a startevent
					//node = new StartEventNode();
					node = new TaskNode();
					if (bflow.startevent == null) {
						bflow.startevent = (TaskNode)node;
						bflow.tasks.add((TaskNode)node);
					}
					break;
				case "bpmn2:endEvent":
					//node = new EndEventNode();
					node = new TaskNode();
					bflow.tasks.add((TaskNode)node);
					break;
				default:
					print nodeType;
				}
				if (isEdge != true && node != null) {
					processNode(node);
					allNodes.add(node);
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
//					print nodeType;
				}
			}
		} /* while */
		return bflow;
	} 

	void showNodeTree () {
		int len = |allNodes|;
		for(int i = 0; i < len; i++) {
			BPMN2Node node = allNodes[i];
			OUT <<< "[" <<< node.class <<< "] ";
			node.printKeys();
		}
	}
}

int main(String[] args)
{
	BPMNParser bparser;
	int argc = |args|;
	print argc;
	if (argc < 1) {
		OUT.println("usage: konoha parser.k $(filename)");
		return 0;
	}
	print args[0];
	String filename = args[0];
	bparser = new BPMNParser(filename);
//	bparser.parse();
	
	BPMNFlow bpmn_flow = bparser.genFlow();
	ous = new OutputStream("code.k", "w");
	ous <<< bpmn_flow.generateCode() <<< EOL;
	ous.close();
	return 0;
}
