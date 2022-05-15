struct DeadKeyTable {
	int tablesCount;
	struct {
		int deadCode;
		int translationCount;
		struct {
			int from;
			int to;
		} translations[16];
	} tables[];
};
