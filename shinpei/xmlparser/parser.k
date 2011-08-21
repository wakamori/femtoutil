#!/usr/local/bin/konoha

using konoha.xml.*;


class BPMN2Node;
class DefinitionsNode extends BPMN2Node {
	String xsi;
	String bpmn2;
	String dc;
	String di;
	String id;

	DefinitionsNode () {
		xsi = "";
		bpmn2 = "";
		dc = "";
		di = "";
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
//			OUT <<< "type=" <<< reader.constName() <<< EOL;
			String nodeType = reader.constName();
			BPMN2Node node;

			switch (nodeType) {
			case "bpmn2:definitions":
				DefinitionsNode n2 = new DefinitionsNode();
				print reader.hasAttributes();
				print reader.getAttributeCount();
				print reader.moveToNextAttribute();
				type = reader.nodeType();
				print "depth=" + reader.depth;
				if (type == XmlReader.TYPE_ELEMENT) {
					print "element";
				} else {
					print type;
					print XmlReader.TYPE_ATTRIBUTE;
				}

				print reader.hasAttributes();
				print reader.getAttributeCount();
				print "Name:" + reader.constName() + ",Value:" + reader.constValue();
				n2.xsi = reader.constValue();
				reader.moveToNextAttribute();
				print "Name:" + reader.constName() + ",Value:" + reader.constValue();
				n2.bpmn2 = reader.constValue();
				reader.moveToNextAttribute();
				n2.dc = reader.constValue();
				n2.di = reader.constValue();
				n2.id = reader.constValue();

				break;
			case "bpmn2:task":
				break;
			case "bpmn2:collaboration":
				break;
			case "bpmn2:process":
				print "hi";
				break;
			}

/*
			if(reader.hasAttributes() && reader.getAttributeCount() > 0){
				while(reader.moveToNextAttribute()){
					OUT << reader.constName() + "=" + reader.constValue() << EOL;
				}
			}
			if(type == XmlReader.TYPE_ELEMENT && reader.constName() == "title"){
				reader.read();
				if(reader.nodeType() == XmlReader.TYPE_TEXT){
					OUT  << reader.constValue() << EOL;
				}
			} else if(type == XmlReader.TYPE_ATTRIBUTE){
				OUT << "att:" + reader.getAttribute() << EOL;
			}
*/
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
