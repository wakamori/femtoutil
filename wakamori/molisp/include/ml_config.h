#define ML_VERSION_MAJOR	1
#define ML_VERSION_MINOR	0
#define MODE_DEBUG			1

/* max input text length */
#define MAX_INPUT_LEN		256

/* max filename length */
#define MAX_FNAME_LEN		256

/* prompt text */
#define PROMPT				"> "

/* prompt text (continue) */
#define PROMPT_CONT			". "

/* exit command */
#define EXIT_STRING			"(quit)"

/* memory pool size */
#define MPOOL_SIZE			(64 * 1024)
#define MPOOL_ALIGN_SIZE	(8)

/* set NDEBUG macro */
#ifndef MODE_DEBUG
#define NDEBUG				1
#endif
