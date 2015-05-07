#ifndef Peder_Hash_COMMIT
#define Peder_Hash_COMMIT


#include "../gmpUtils.c"
#include "../DDH_Primitive.h"
#include "../../comms/sockets.h"


typedef struct Peder_Hash_commit_box
{
	struct eccPoint *u;
	struct eccPoint *v;
}  Peder_Hash_commit_box;


typedef struct Peder_Hash_commit_key
{
	mpz_t r;
	unsigned char *x;
	int xLen;
}  Peder_Hash_commit_key;

typedef struct commit_batch_params
{
	struct eccParams *params;
	struct eccPoint *h;
}  commit_batch_params;




struct commit_batch_params *init_commit_batch_params();
struct commit_batch_params *generate_commit_params(int securityParam, gmp_randstate_t state);


struct Peder_Hash_commit_box *init_commit_box();
struct Peder_Hash_commit_key *init_commit_key();

int sizeOf_C_Box_Array(struct Peder_Hash_commit_box **c, int numC_Boxes);
int sizeOf_K_Box_Array(struct Peder_Hash_commit_key **k, int msgLength, int numK_Boxes);

void serialise_Peder_Hash_Cbox(struct Peder_Hash_commit_box *c, unsigned char *outputBuffer, int *bufferOffset);
struct Peder_Hash_commit_box *deserialise_Peder_Hash_Cbox(unsigned char *inputBuffer, int *bufferOffset);

void serialise_Peder_Hash_Kbox(struct Peder_Hash_commit_key *k, int msgLength, unsigned char *outputBuffer, int *bufferOffset);
struct Peder_Hash_commit_key *deserialise_Peder_Hash_Kbox(unsigned char *inputBuffer, int msgLength, int *bufferOffset);


unsigned char *serialiseAllC_Boxes(struct Peder_Hash_commit_box **c, int numC_Boxes, int *totalLength);
struct Peder_Hash_commit_box **deserialiseAllC_Boxes(unsigned char *inputBuffer, int numC_Boxes, int *bufferOffset);


unsigned char *serialiseAllK_Boxes(struct Peder_Hash_commit_key **k, int msgLength, int numK_Boxes, int *totalLength);
struct Peder_Hash_commit_key **deserialiseAllK_Boxes(unsigned char *inputBuffer, int msgLength, int numK_Boxes, int *bufferOffset);



void create_commit_box_key(struct commit_batch_params *params, unsigned char *toCommit, int toCommitLen, gmp_randstate_t state,
						struct Peder_Hash_commit_box *c, struct Peder_Hash_commit_key *k);


struct Peder_Hash_commit_key *single_commit_Peder_Hash_C(struct commit_batch_params *params,
												unsigned char *toCommit, int toCommitLen,
												unsigned char *outputBuffer, int *bufferOffset,
												gmp_randstate_t state);


struct Peder_Hash_commit_box *single_commit_Peder_Hash_R(struct commit_batch_params *params, unsigned char *inputBuffer, int *bufferOffset);
unsigned char *single_decommit_Peder_Hash_R(struct commit_batch_params *params, struct Peder_Hash_commit_box *c, struct Peder_Hash_commit_key *k, int msgLength, int *outputLength);


int send_commit_batch_params(int writeSocket, int readSocket, struct commit_batch_params *params);
struct commit_batch_params *receive_commit_batch_params(int writeSocket, int readSocket);

struct commit_batch_params *setup_Peder_Hash_C(int writeSocket, int readSocket, int securityParam, gmp_randstate_t state);
struct commit_batch_params *setup_Peder_Hash_R(int writeSocket, int readSocket);



#include "commit_Peder_Hash_utils.c"
#include "commit_Peder_Hash.c"


#endif