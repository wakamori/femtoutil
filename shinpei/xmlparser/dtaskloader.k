#!/usr/local/bin/konoha

class DTaskLoader {
	String searchFolderName;
	
	DTaskLoader (String fn) {
		searchFolderName = fn;
	}

	void setSerachFolder (String fn) {
		searchFolderName = fn;
	}

	String fetch(String taskname) {
		print taskname;
		InputStream ins = new InputStream(searchFolderName + "/" + taskname + ".k");
		String content = "";
		foreach (String line in ins) {
			if (|line| > 0 && line[0] != "#") {
				content += line + "\n";
			}
		}
		return content;

	}
}
