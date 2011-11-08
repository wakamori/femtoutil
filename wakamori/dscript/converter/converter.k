void main(String[] args)
{
	assert |args| == 1;
	assert not args[0].endsWith(".k");
	Path source = args[0];
	Path gen = args[0].replace($/(.*)\..*$/, "$1.k");
	InputStream ins = new InputStream(source, "r");
	String result = "";
	String actor_name = "";
	int b_count = 0;
	boolean isInnerTask = false;
	foreach(String line in ins) {
		if (|line.match($/actor:[a-z0-9\.]+:\d+/)| > 0) {
			actor_name = line.split("=")[0].trim();
		}
		if (|line.match(new Regex(actor_name + "->task\\s*\"[a-zA-Z0-9\\.\\-_]+\"\\s*{"))| > 0) {
			line = actor_name + ".startTask(\"" + line.replace($/.*->task\s*"([a-zA-Z0-9\.\-_]+)".*$/, "$1") + "\");";
			line += EOL + actor_name + ".eval(\"\"\"";
			b_count++;
			isInnerTask = true;
			result += line;
			continue;
		} else if (line.indexOf("{") != -1) {
			b_count = b_count + |line.match($/{/g)|;
		}
		if (line.indexOf("}") != -1) {
			b_count = b_count - |line.match($/}/g)|;
			if (b_count == 0 && isInnerTask) {
				int index = line.lastIndexOf("}");
				if (|line| > index + 1) {
					line = line[0 until index] + " " + line[index + 1 to];
				} else if (index != 0) {
					line = line[0 until index];
				} else {
					line = "";
				}
				line += "\"\"\");" + EOL;
				line += actor_name + ".endTask();";
				isInnerTask = false;
			}
		}
		result += line + EOL;
	}
	OutputStream ous = new OutputStream(gen, "w");
	ous << result << EOL;
	ous.close();
}
