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

