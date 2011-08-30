#!/usr/local/bin/konoha

using konoha.xml.*;


class BPMN2Node {
	int type;
}

/* types */
NODETYPE_DEF = 1;
NODETYPE_COLLABOLATION = 2;
NODETYPE_PARTICIPANT = 3;
NODETYPE_PROCESS = 4;

class DefinitionsNode extends BPMN2Node {
	String xsi;
	String bpmn2;
	String dc;
	String di;
	String id;

	DefinitionsNode () {
		type = 
		xsi = "";
		bpmn2 = "";
		dc = "";
		di = "";
		id = "";
	}
}

class CollaborationNode extends BPMN2Node {
	String id;
	CollaborationNode () {
		id = "";
	}
}



class TaskNode extends BPMN2Node;
class ProcessNode extends BPMN2Node;

class DReader {
	XmlReader reader;
	String filename;

	DReader (String xmlfile) {
		filename = xmlfile;
		reader = new XmlReader(filename);
	}

	void doProcess () {
		while(reader.read()){
			type = reader.nodeType();
			String nodeType = reader.constName();
			BPMN2Node node;
			print nodeType;
			switch (nodeType) {
			case "bpmn2:definitions":
				node = new DefinitionsNode();
				DefinitionsNode n2 = (DefinitionsNode)node;
				int attrCount = 0;
				if (reader.hasAttributes()) {
					attrCount = reader.getAttributeCount();
				}
				reader.moveToNextAttribute();
				if (reader.nodeType() == XmlReader.TYPE_ATTRIBUTE) {
					n2.xsi = reader.constValue();
					print reader.constName();
				} else {
					print "error!";
					break;
				}

				reader.moveToNextAttribute();
				if (reader.nodeType() == XmlReader.TYPE_ATTRIBUTE) {
					n2.bpmn2 = reader.constValue();
					print reader.constName();
				} else {
					print "error!";
					break;
				}

				reader.moveToNextAttribute();
				if (reader.nodeType() == XmlReader.TYPE_ATTRIBUTE) {
					n2.dc = reader.constValue();
				} else {
					print "error!";
					break;
				}
				reader.moveToNextAttribute();
				if (reader.nodeType() == XmlReader.TYPE_ATTRIBUTE) {
					n2.di = reader.constValue();
				} else {
					print "error!";
					break;
				}
				reader.moveToNextAttribute();
				if (reader.nodeType() == XmlReader.TYPE_ATTRIBUTE) {
					n2.id = reader.constValue();
				} else {
					print "error!";
					break;
				}

				break;
			case "bpmn2:collaboration":
				node = new CollaborationNode();
				CollaborationNode n2 = (CollaborationNode)node;
				int attrCount = 0;
				if (reader.hasAttributes()) {
					attrCount = reader.getAttributeCount();
				}
				reader.moveToNextAttribute();
				if (reader.nodeType() == XmlReader.TYPE_ATTRIBUTE) {
					n2.id = reader.constValue();
				}
				break;
			case "bpmn2:participant":
				
				
			case "bpmn2:task":
				break;
			case "bpmn2:process":
				break;
			}
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
	return 0;
}
