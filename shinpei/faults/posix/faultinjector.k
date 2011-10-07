/* test environment setup  */

class FaultSetup {
	@Public void createFileWithSize (String fname, int fsize) {
		ous = new OutputStream(fname, "w");
		int size = 0;
		while (size < fsize) {
			ous << "@";
			size++;
		}
		ous.close();
	}
}

class FaultInstance {
	String faultName;
	@Virtual boolean inject(void);
}

class TargetInstance;

class FaultInjector {
	Array<FaultInstance> farray;
	TargetInstance target;

	FaultInjector () {
		farray = [];
	}

	void addFaultInstance(FaultInstance fi) {
		farray.add(fi);
	}

	int loadToLocalTarget() {
		int i = 0;
		Array<boolean> results = [];

		foreach (FaultInstance f in farray) {
			results.add(f.inject());
		}
		print results;
		return 0;
	}

	int loadToTarget () {
		return 0;
	}
}

class InjectionScenario {
	int timing;
	@Virtual void start();
	@Virtual void end();
}

