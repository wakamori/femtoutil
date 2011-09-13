/* -------------------------------------------
 * bpmn2.k : 
 *  bpmn node definitions
 *  shinpei(c)2011
 *
 * ------------------------------------------- */

include "dtaskloader.k";

class BPMN2Object {
	Map <String, String> attributes;
}

class FlowObject extends BPMN2Object {
	Array<FlowObject> source;
	Array<FlowObject> target;
}


class Event extends FlowObject {
	Event () {
		attributes = {};
		source = [];
		target = [];
	}
}

class Activity extends FlowObject {
	Activity () { 
		attributes = {};
		source = [];
		target = [];
	}
}

class UserActivity extends Activity {
	UserActivity () {
		attributes = {};
		source = [];
		target = [];

	}

}

class ScriptActivity extends Activity {
	ScriptActivity () {
		attributes = {};
		source = [];
		target = [];
	}
}

class Gateway extends FlowObject {
	Gateway () {
		attributes = {};
		source = [];
		target = [];
	}
}

class ConnectionObject extends BPMN2Object;

class SequenceFlow extends ConnectionObject {
	SequenceFlow () { attributes = {}; }
}

class MessageFlow extends ConnectionObject {
	MessageFlow() { attributes = {}; }
}

class Association extends ConnectionObject {
	Association () { attributes = {}; }
}


//class DataObject extends BPMN2Object;
//class Group extends BPMN2Object;
//class Annotation extends BPMN2Object;



class BPMNWorkflow {
	@Private Event startEvent;
	@Private Event endEvent;

	@Private Array<FlowObject> flowobjects;
	int flowNum;
	@Private Array<ConnectionObject> connobjects;
	int connNum;

	FlowObject seekPointer;

	BPMNWorkflow () {
		flowobjects = [];
		connobjects = [];
		flowNum = 0;
		connNum = 0;
		seekPointer = startEvent;
	}

	@Public Event fetchStartEvent () {
		return startEvent;
	}

	@Public void addFlow (FlowObject fo) {
		flowobjects.add(fo);
		flowNum++;
	}

	@Public void addConnection(ConnectionObject co) {
		connobjects.add(co);
		connNum++;
	}

	@Public int fetchNumOfBranchDirections () {
		return |seekPointer.target|;
	}

	@Public FlowObject fetchNextObject (int idx = 0) {
		return seekPointer.target[idx];
	}
}

class CodeGenerator {
	DtaskLoader dtl;
	String dtask_dirname;

	CodeGenerator (String dirname) {
		dtask_dirname = dirname;
		dtl = new DtaskLoader(dtask_dirname);
	}

	String generate (BPMNWorkflow workflow) {
		String code = '';
		if (workflow.flowNum == 0 || workflow.connNum == 0) return "";
		
		// task parse
		Map <String, Activity> activity_map = {};

		

		return code;

	}
	
}




// XML object
class BPMN2Node {
	Map<String, String> attributes;

	void printKeys () {
		String[] keys = attributes.keys();
		foreach(String key in keys) {
			OUT <<< key <<< "," <<<  attributes[key] <<< EOL;
		}
	}
}

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

class TaskNode extends BPMN2Node {
	Array<TaskNode> referenceFrom;
	Array<TaskNode> referenceTo;
	String taskname;
	TaskNode () {
		attributes = {};
		referenceFrom = [];
		referenceTo = [];
	}
}

class SequenceFlowNode extends BPMN2Node {
	TaskNode source;
	TaskNode target;
	// TODO;

	SequenceFlowNode () {
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

