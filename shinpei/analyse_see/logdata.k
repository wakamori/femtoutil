class AnalyzeData;

class LogRawData extends AnalyzeData {
	String event;
    int time;
	int address;
	String s_address;
	int i_data;
	String s_data;
	Int idx;

	LogRawData(String ev) {
		_event = ev;
	}
}

class FastMallocData extends LogRawData;
class FastFreeData extends LogRawData;
class FastReallocData extends LogRawData;
class VallocData extends LogRawData;
class VfreeData extends LogRawData;
class CreateObjectData extends LogRawData;
class DeleteObjectData extends LogRawData;
class ClassNameData extends LogRawData;
