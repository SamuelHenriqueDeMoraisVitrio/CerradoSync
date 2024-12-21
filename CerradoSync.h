#ifndef CerradoSync_imp
#define CerradoSync_imp

#define _GNU_SOURCE
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>






#endif

#ifndef CerradoSync_depG
#define CerradoSync_depG

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end

#ifndef SHA_256_H
#define SHA_256_H

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief Size of the SHA-256 sum. This times eight is 256 bits.
 */
#define SIZE_OF_SHA_256_HASH 32

/*
 * @brief Size of the chunks used for the calculations.
 *
 * @note This should mostly be ignored by the user, although when using the streaming API, it has an impact for
 * performance. Add chunks whose size is a multiple of this, and you will avoid a lot of superfluous copying in RAM!
 */
#define SIZE_OF_SHA_256_CHUNK 64

/*
 * @brief The opaque SHA-256 type, that should be instantiated when using the streaming API.
 *
 * @note Although the details are exposed here, in order to make instantiation easy, you should refrain from directly
 * accessing the fields, as they may change in the future.
 */
struct Sha_256 {
	uint8_t *hash;
	uint8_t chunk[SIZE_OF_SHA_256_CHUNK];
	uint8_t *chunk_pos;
	size_t space_left;
	uint64_t total_len;
	uint32_t h[8];
};

/*
 * @brief The simple SHA-256 calculation function.
 * @param hash Hash array, where the result is delivered.
 * @param input Pointer to the data the hash shall be calculated on.
 * @param len Length of the input data, in byte.
 *
 * @note If all of the data you are calculating the hash value on is available in a contiguous buffer in memory, this is
 * the function you should use.
 *
 * @note If either of the passed pointers is NULL, the results are unpredictable.
 *
 * @note See note about maximum data length for sha_256_write, as it applies for this function's len argument too.
 */
void calc_sha_256(uint8_t hash[SIZE_OF_SHA_256_HASH], const void *input, size_t len);

/*
 * @brief Initialize a SHA-256 streaming calculation.
 * @param sha_256 A pointer to a SHA-256 structure.
 * @param hash Hash array, where the result will be delivered.
 *
 * @note If all of the data you are calculating the hash value on is not available in a contiguous buffer in memory,
 * this is where you should start. Instantiate a SHA-256 structure, for instance by simply declaring it locally, make
 * your hash buffer available, and invoke this function. Once a SHA-256 hash has been calculated (see further below) a
 * SHA-256 structure can be initialized again for the next calculation.
 *
 * @note If either of the passed pointers is NULL, the results are unpredictable.
 */
void sha_256_init(struct Sha_256 *sha_256, uint8_t hash[SIZE_OF_SHA_256_HASH]);

/*
 * @brief Stream more input data for an on-going SHA-256 calculation.
 * @param sha_256 A pointer to a previously initialized SHA-256 structure.
 * @param data Pointer to the data to be added to the calculation.
 * @param len Length of the data to add, in byte.
 *
 * @note This function may be invoked an arbitrary number of times between initialization and closing, but the maximum
 * data length is limited by the SHA-256 algorithm: the total number of bits (i.e. the total number of bytes times
 * eight) must be representable by a 64-bit unsigned integer. While that is not a practical limitation, the results are
 * unpredictable if that limit is exceeded.
 *
 * @note This function may be invoked on empty data (zero length), although that obviously will not add any data.
 *
 * @note If either of the passed pointers is NULL, the results are unpredictable.
 */
void sha_256_write(struct Sha_256 *sha_256, const void *data, size_t len);

/*
 * @brief Conclude a SHA-256 streaming calculation, making the hash value available.
 * @param sha_256 A pointer to a previously initialized SHA-256 structure.
 * @return Pointer to the hash array, where the result is delivered.
 *
 * @note After this function has been invoked, the result is available in the hash buffer that initially was provided. A
 * pointer to the hash value is returned for convenience, but you should feel free to ignore it: it is simply a pointer
 * to the first byte of your initially provided hash array.
 *
 * @note If the passed pointer is NULL, the results are unpredictable.
 *
 * @note Invoking this function for a calculation with no data (the writing function has never been invoked, or it only
 * has been invoked with empty data) is legal. It will calculate the SHA-256 value of the empty string.
 */
uint8_t *sha_256_close(struct Sha_256 *sha_256);

#ifdef __cplusplus
}
#endif

#endif


#endif

#ifndef CerradoSync_depH
#define CerradoSync_depH

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end

#define TOTAL_LEN_LEN 8

/*
 * Comments from pseudo-code at https://en.wikipedia.org/wiki/SHA-2 are reproduced here.
 * When useful for clarification, portions of the pseudo-code are reproduced here too.
 */

/*
 * @brief Rotate a 32-bit value by a number of bits to the right.
 * @param value The value to be rotated.
 * @param count The number of bits to rotate by.
 * @return The rotated value.
 */
static inline uint32_t right_rot(uint32_t value, unsigned int count)
{
	/*
	 * Defined behaviour in standard C for all count where 0 < count < 32, which is what we need here.
	 */
	return value >> count | value << (32 - count);
}

/*
 * @brief Update a hash value under calculation with a new chunk of data.
 * @param h Pointer to the first hash item, of a total of eight.
 * @param p Pointer to the chunk data, which has a standard length.
 *
 * @note This is the SHA-256 work horse.
 */
static inline void consume_chunk(uint32_t *h, const uint8_t *p)
{
	unsigned i, j;
	uint32_t ah[8];

	/* Initialize working variables to current hash value: */
	for (i = 0; i < 8; i++)
		ah[i] = h[i];

	/*
	 * The w-array is really w[64], but since we only need 16 of them at a time, we save stack by
	 * calculating 16 at a time.
	 *
	 * This optimization was not there initially and the rest of the comments about w[64] are kept in their
	 * initial state.
	 */

	/*
	 * create a 64-entry message schedule array w[0..63] of 32-bit words (The initial values in w[0..63]
	 * don't matter, so many implementations zero them here) copy chunk into first 16 words w[0..15] of the
	 * message schedule array
	 */
	uint32_t w[16];

	/* Compression function main loop: */
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 16; j++) {
			if (i == 0) {
				w[j] =
				    (uint32_t)p[0] << 24 | (uint32_t)p[1] << 16 | (uint32_t)p[2] << 8 | (uint32_t)p[3];
				p += 4;
			} else {
				/* Extend the first 16 words into the remaining 48 words w[16..63] of the
				 * message schedule array: */
				const uint32_t s0 = right_rot(w[(j + 1) & 0xf], 7) ^ right_rot(w[(j + 1) & 0xf], 18) ^
						    (w[(j + 1) & 0xf] >> 3);
				const uint32_t s1 = right_rot(w[(j + 14) & 0xf], 17) ^
						    right_rot(w[(j + 14) & 0xf], 19) ^ (w[(j + 14) & 0xf] >> 10);
				w[j] = w[j] + s0 + w[(j + 9) & 0xf] + s1;
			}
			const uint32_t s1 = right_rot(ah[4], 6) ^ right_rot(ah[4], 11) ^ right_rot(ah[4], 25);
			const uint32_t ch = (ah[4] & ah[5]) ^ (~ah[4] & ah[6]);

			/*
			 * Initialize array of round constants:
			 * (first 32 bits of the fractional parts of the cube roots of the first 64 primes 2..311):
			 */
			static const uint32_t k[] = {
			    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4,
			    0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe,
			    0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f,
			    0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
			    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc,
			    0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
			    0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116,
			    0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
			    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7,
			    0xc67178f2};

			const uint32_t temp1 = ah[7] + s1 + ch + k[i << 4 | j] + w[j];
			const uint32_t s0 = right_rot(ah[0], 2) ^ right_rot(ah[0], 13) ^ right_rot(ah[0], 22);
			const uint32_t maj = (ah[0] & ah[1]) ^ (ah[0] & ah[2]) ^ (ah[1] & ah[2]);
			const uint32_t temp2 = s0 + maj;

			ah[7] = ah[6];
			ah[6] = ah[5];
			ah[5] = ah[4];
			ah[4] = ah[3] + temp1;
			ah[3] = ah[2];
			ah[2] = ah[1];
			ah[1] = ah[0];
			ah[0] = temp1 + temp2;
		}
	}

	/* Add the compressed chunk to the current hash value: */
	for (i = 0; i < 8; i++)
		h[i] += ah[i];
}

/*
 * Public functions. See header file for documentation.
 */

void sha_256_init(struct Sha_256 *sha_256, uint8_t hash[SIZE_OF_SHA_256_HASH])
{
	sha_256->hash = hash;
	sha_256->chunk_pos = sha_256->chunk;
	sha_256->space_left = SIZE_OF_SHA_256_CHUNK;
	sha_256->total_len = 0;
	/*
	 * Initialize hash values (first 32 bits of the fractional parts of the square roots of the first 8 primes
	 * 2..19):
	 */
	sha_256->h[0] = 0x6a09e667;
	sha_256->h[1] = 0xbb67ae85;
	sha_256->h[2] = 0x3c6ef372;
	sha_256->h[3] = 0xa54ff53a;
	sha_256->h[4] = 0x510e527f;
	sha_256->h[5] = 0x9b05688c;
	sha_256->h[6] = 0x1f83d9ab;
	sha_256->h[7] = 0x5be0cd19;
}

void sha_256_write(struct Sha_256 *sha_256, const void *data, size_t len)
{
	sha_256->total_len += len;

	/*
	 * The following cast is not necessary, and could even be considered as poor practice. However, it makes this
	 * file valid C++, which could be a good thing for some use cases.
	 */
	const uint8_t *p = (const uint8_t *)data;

	while (len > 0) {
		/*
		 * If the input chunks have sizes that are multiples of the calculation chunk size, no copies are
		 * necessary. We operate directly on the input data instead.
		 */
		if (sha_256->space_left == SIZE_OF_SHA_256_CHUNK && len >= SIZE_OF_SHA_256_CHUNK) {
			consume_chunk(sha_256->h, p);
			len -= SIZE_OF_SHA_256_CHUNK;
			p += SIZE_OF_SHA_256_CHUNK;
			continue;
		}
		/* General case, no particular optimization. */
		const size_t consumed_len = len < sha_256->space_left ? len : sha_256->space_left;
		memcpy(sha_256->chunk_pos, p, consumed_len);
		sha_256->space_left -= consumed_len;
		len -= consumed_len;
		p += consumed_len;
		if (sha_256->space_left == 0) {
			consume_chunk(sha_256->h, sha_256->chunk);
			sha_256->chunk_pos = sha_256->chunk;
			sha_256->space_left = SIZE_OF_SHA_256_CHUNK;
		} else {
			sha_256->chunk_pos += consumed_len;
		}
	}
}

uint8_t *sha_256_close(struct Sha_256 *sha_256)
{
	uint8_t *pos = sha_256->chunk_pos;
	size_t space_left = sha_256->space_left;
	uint32_t *const h = sha_256->h;

	/*
	 * The current chunk cannot be full. Otherwise, it would already have been consumed. I.e. there is space left
	 * for at least one byte. The next step in the calculation is to add a single one-bit to the data.
	 */
	*pos++ = 0x80;
	--space_left;

	/*
	 * Now, the last step is to add the total data length at the end of the last chunk, and zero padding before
	 * that. But we do not necessarily have enough space left. If not, we pad the current chunk with zeroes, and add
	 * an extra chunk at the end.
	 */
	if (space_left < TOTAL_LEN_LEN) {
		memset(pos, 0x00, space_left);
		consume_chunk(h, sha_256->chunk);
		pos = sha_256->chunk;
		space_left = SIZE_OF_SHA_256_CHUNK;
	}
	const size_t left = space_left - TOTAL_LEN_LEN;
	memset(pos, 0x00, left);
	pos += left;
	uint64_t len = sha_256->total_len;
	pos[7] = (uint8_t)(len << 3);
	len >>= 5;
	int i;
	for (i = 6; i >= 0; --i) {
		pos[i] = (uint8_t)len;
		len >>= 8;
	}
	consume_chunk(h, sha_256->chunk);
	/* Produce the final hash value (big-endian): */
	int j;
	uint8_t *const hash = sha_256->hash;
	for (i = 0, j = 0; i < 8; i++) {
		hash[j++] = (uint8_t)(h[i] >> 24);
		hash[j++] = (uint8_t)(h[i] >> 16);
		hash[j++] = (uint8_t)(h[i] >> 8);
		hash[j++] = (uint8_t)h[i];
	}
	return sha_256->hash;
}

void calc_sha_256(uint8_t hash[SIZE_OF_SHA_256_HASH], const void *input, size_t len)
{
	struct Sha_256 sha_256;
	sha_256_init(&sha_256, hash);
	sha_256_write(&sha_256, input, len);
	(void)sha_256_close(&sha_256);
}


#endif

#ifndef CerradoSync_mac
#define CerradoSync_mac

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



#define _SIZE_STACK_PROCESS_1MB_ 1024 * 1024 // 1mb

#define _DEFAULT_MAX_SIZE_MEMORY_TRAFFIC_ 1024 // 1kb

#define _WRITE_AND_READ_ 0666 // Read and write permission

#define _CONFIG_SHMGET_PERMISSIONS_ _WRITE_AND_READ_ | IPC_CREAT | IPC_EXCL // shmget configuration function



//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



#define GREEN_TRAFFIC 1
#define RED_TRAFFIC 0






#endif

#ifndef CerradoSync_typesE
#define CerradoSync_typesE

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end


typedef struct Sha_256 Hash;

typedef struct shmid_ds ShmidDS;



//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end

typedef struct ProcessStruct Process;

typedef struct CerradoSynStruct CerradoSyn;

typedef struct Arguments_struct ArgumentsCallback;

typedef struct Argument_struct ArgumentCallback;

typedef struct CallbackProcess_struct CallbackProcess;

typedef struct MemorySharedContent_struct MemorySharedContent;

typedef struct MemoryShared_struct MemoryShared;

typedef struct TrafficPointersList_struct TrafficPointersList;

typedef struct TrafficPointerObject_struct TrafficPointerObject;

typedef struct TrafficMemory_struct TrafficMemory;



#endif

#ifndef CerradoSync_typesF
#define CerradoSync_typesF

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



struct Arguments_struct{
  int size_arguments;
  ArgumentCallback **arguments;
};

struct Argument_struct{
  void *arg;
  const char *name_argument;
};





#endif

#ifndef CerradoSync_typesG
#define CerradoSync_typesG

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end





struct CallbackProcess_struct{
  int(*function_callback)(MemoryShared *memory, ArgumentsCallback *arguments);
  ArgumentsCallback *args;
  MemoryShared *memory;
};





//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end


struct MemorySharedContent_struct{
  void *memoryShared;
  void *memory;
  size_t size_memory;
  size_t size_memoryShared;
  TrafficMemory *traffic;
};

struct MemoryShared_struct{
  MemorySharedContent *memory_shared;
  key_t key;
  pid_t pid;
  int memory_location;
  TrafficPointersList *traffic;
};





//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end


struct ProcessStruct{
  pid_t process;
  void *stack;
  int size_stack;
};








//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end

struct TrafficPointerObject_struct{
  int traffic_ID;
  int number_traffics;
  key_t key;
  const char *nameClass;
};

struct TrafficPointersList_struct{
  int size_elements;
  TrafficPointerObject **semID;
};

struct TrafficMemory_struct{
  int trafficID;
  key_t key;
};





#endif

#ifndef CerradoSync_typesH
#define CerradoSync_typesH

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end


struct CerradoSynStruct{
  int pid_father;
  const char *name_class;
  key_t key;

  Process **process_list;
  size_t size_process;
  
  MemoryShared *memory;
  CerradoSyn *class_list;
};







#endif

#ifndef CerradoSync_dec
#define CerradoSync_dec

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end


int private_free_interrupted(void *arg_for_verify, void **args_for_free, size_t size_args);






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



key_t private_generate_string_key(const char *key, pid_t hierarchy);

key_t private_creat_key(const char *key);

key_t private_get_key(const char *key);





//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



int private_clone_process(Process *process, CallbackProcess *callback, int *flags);





//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end


int create_process(CerradoSyn *main_process, CallbackProcess *callback, int *flags);




//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



int private_memory_data_attach(MemoryShared *memory_shared);

ShmidDS *get_info_memory_location(MemoryShared *memory_shared);

void private_close_memory(MemoryShared *memory_shared);

void pull_memory(MemorySharedContent *self);

void push_memory(MemorySharedContent *self);

void config_memory(MemorySharedContent *self, void *new_value, size_t size_value);

void private_config_memory_share(MemorySharedContent *self);






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



int private_get_stats_traffic(int id, int index_get);

int private_init_traffic(key_t key);

int private_creat_a_wait_point(const char *className, int initial, int number_traffics, key_t *key);

int private_wait(key_t key, int number_traffics, int index_get, int color);

int private_signal_traffic(int id, int index_traffic, int color);

void private_close_traffic(int id);

int create_pointer_traffic(CerradoSyn *self, const char *className, int initial_pointer);

int wait_traffic(MemoryShared *memory, const char *className, int color);

int signal_traffic(MemoryShared *memory, const char *className, int color);











//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



CallbackProcess *new_CallbackProcess(CerradoSyn *process_father, int (*function)(MemoryShared *memory, ArgumentsCallback *arguments));

ArgumentCallback *new_argument(const char *name_argument, void *arg, size_t arg_size);

void private_free_argument(ArgumentCallback *self);

void free_callback(CallbackProcess *self);

int add_argument(CallbackProcess *callback_self, ArgumentCallback *add_arg);

ArgumentsCallback *private_new_ArgumentsCallback();

void private_free_ArgumentsCallback(ArgumentsCallback *self);







//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end




CerradoSyn *new_CerradoSynStruct(const char *class_name, size_t size_max_memory_traffic);

void free_CerradoSyn(CerradoSyn *self);






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



MemoryShared *private_new_MemorySahred_struct(const char *name_class, size_t size_max_traffic);

void private_delet_memory(MemoryShared *memory_shared);





//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end


MemorySharedContent *private_new_MemorySharedContent(MemoryShared *memory_struct, size_t size_memoryShared);

void private_free_MemorySharedContent(MemorySharedContent *self);




//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



TrafficMemory *private_new_TrafficMemory(key_t key);

TrafficPointerObject *private_new_TrafficPointerObject(const char *className, int contTraffics, int initialPointer);

TrafficPointersList *private_new_TrafficPointersList();

void private_free_TrafficPointerObject(TrafficPointerObject *self);

void private_free_TrafficPointersList(TrafficPointersList *self);

void private_free_traffic(TrafficMemory *self);





//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



Process *private_new_process(int size_stack);

void private_free_process(Process *self);





#endif

#ifndef CerradoSync_def
#define CerradoSync_def

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



int private_free_interrupted(void *arg_for_verify, void **args_for_free, size_t size_args){

  if(arg_for_verify != NULL){
    return 1;
  }

  for(int i = 0; i < size_args; i++){
    if((args_for_free[i]) == NULL){
      free(args_for_free[i]);
    }
  }

  return 0;
}






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



key_t private_generate_string_key(const char *key, pid_t hierarchy){
  if(key == NULL || !(strlen(key) > 0) || !(hierarchy > 0)){
    return 0;
  }

  short size_pid = 15;
  long size_key_formated = strlen(key) + size_pid + 3;

  char key_hash[size_key_formated];
  snprintf(key_hash, size_key_formated, "%s-%d", key, hierarchy);

  uint8_t hash[SIZE_OF_SHA_256_HASH];
  calc_sha_256(hash, key_hash, strlen(key_hash));

  key_t numeric_key = 0;
  for (int i = 0; i < SIZE_OF_SHA_256_HASH; i++) {
    numeric_key = numeric_key ^ (hash[i] << ((i % sizeof(key_t)) * 8));
  }

  return numeric_key;
}

key_t private_creat_key(const char *key){
  return private_generate_string_key(key, getpid());
}

key_t private_get_key(const char *key){
  return private_generate_string_key(key, getppid());
}







//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end

int callback_config(void *arg){

  CallbackProcess *struct_arg = (CallbackProcess *)arg;

  MemoryShared *memory_argument_0 = (MemoryShared *)struct_arg->memory;
  private_memory_data_attach(memory_argument_0);

  pull_memory(memory_argument_0->memory_shared);

  int(*function_callback)(MemoryShared *memory, ArgumentsCallback *arguments);
  function_callback = struct_arg->function_callback;
  int var_return = function_callback(memory_argument_0, struct_arg->args);

  private_close_memory(memory_argument_0);

  return var_return;
}

int private_clone_process(Process *process, CallbackProcess *callback, int *flags){

  pid_t pid_process = clone(callback_config, process->stack + process->size_stack - 1, flags?*flags:SIGCHLD, callback);

  if(pid_process == -1){
    return -1;
  }

  process->process = pid_process;

  return 1;

}






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end




int create_process(CerradoSyn *main_process, CallbackProcess *callback, int *flags) {
  if (getpid() != main_process->pid_father) {
      return -1;
  }

  if(!main_process || !callback){
    return -1;
  }

  Process *new_process = private_new_process(_SIZE_STACK_PROCESS_1MB_);
  if(!new_process){
    return -1;
  }

  private_clone_process(new_process, callback, flags);
  if(new_process->process == -1){
    private_free_process(new_process);
    return -1;
  }

  main_process->size_process++;

  main_process->process_list = (Process **)realloc(main_process->process_list, (main_process->size_process + 1) * sizeof(Process *));
  if(!main_process->process_list){
    return -1;
  }

  main_process->process_list[main_process->size_process - 1] = new_process;

  return 1;
}






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end




int private_memory_data_attach(MemoryShared *memory_shared){
  void *data = (void *)shmat(memory_shared->memory_location, NULL, 0);
  if(data == (void *)-1){
    return -1;
  }

  memory_shared->memory_shared->memoryShared = data;
  return 1;
}

void private_close_memory(MemoryShared *memory_shared){

  shmdt(memory_shared->memory_shared->memoryShared);
}

ShmidDS *get_info_memory_location(MemoryShared *memory_shared){
  ShmidDS *shmInfo;
  if(shmctl(memory_shared->memory_location, IPC_STAT, shmInfo) == -1){
    return NULL;
  }

  return shmInfo;
}

void pull_memory(MemorySharedContent *self){
  
  private_signal_traffic(self->traffic->trafficID, 0, -1);// pedindo ascesso à memoria;

  config_memory(self, self->memoryShared, self->size_memoryShared);//Lendo

  private_signal_traffic(self->traffic->trafficID, 0, 1);// Entregando ascesso à memoria;

}

void push_memory(MemorySharedContent *self){

  private_signal_traffic(self->traffic->trafficID, 0, -1);// pedindo ascesso à memoria;

  private_config_memory_share(self);// Gravando

  private_signal_traffic(self->traffic->trafficID, 0, 1);// Entregando ascesso à memoria;

}

void config_memory(MemorySharedContent *self, void *new_value, size_t size_value){

  memset(self->memory, 0, self->size_memory);

  self->size_memory = size_value + 1;

  self->memory = (void *)realloc(self->memory, self->size_memory);

  memcpy(self->memory, new_value , size_value);
}

void private_config_memory_share(MemorySharedContent *self){

  memset(self->memoryShared, 0, self->size_memoryShared);

  memcpy(self->memoryShared, self->memory, self->size_memory);

}








//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end




int private_get_stats_traffic(int id, int index_get){
  return semctl(id, index_get, GETVAL);
}

int private_init_traffic(key_t key){

  int sem_share = semget(key, 2, IPC_CREAT | IPC_EXCL | 0666);

  if(sem_share == -1){
    return sem_share;
  }

  if(semctl(sem_share, 0, SETVAL, 1) == -1){// The first set of traffic lights will be dedicated to "memory share";
    return -1;
  }

  if(semctl(sem_share, 1, SETVAL, 0) == -1){// The second set of traffic lights will be dedicated to the "await sync";
    return -1;
  }
  
  return sem_share;
}

int private_creat_a_wait_point(const char *className, int initial, int number_traffics, key_t *key){
  *key = private_creat_key(className);

  int sem_point = semget(*key, 1, IPC_CREAT | IPC_EXCL | 0666);
  if(sem_point == -1){
    return -1;
  }

  for(int i = 0; i < number_traffics; i++){
    if(semctl(sem_point, i, SETVAL, initial) == -1){
      return -1;
    }
  }

  return sem_point;
}

int private_wait(key_t key, int number_traffics, int index_get, int color){

  int sem_point = semget(key, number_traffics, 0);
  if(sem_point == -1){
    return -1;
  }

  int point_stats = private_get_stats_traffic(sem_point, index_get);
  if(point_stats == -1){
    return -1;
  }

  if(color == GREEN_TRAFFIC || color == RED_TRAFFIC){
    
    do{

      point_stats = private_get_stats_traffic(sem_point, index_get);
      if(point_stats == -1){
        return -1;
      }
      usleep(1000);
      continue;
    
    }while((color == GREEN_TRAFFIC && point_stats == 0) || (color == RED_TRAFFIC && point_stats > 0));

    return 1;

  }

  return 0;
}

int private_signal_traffic(int id, int index_traffic, int color){

  struct sembuf operation = {0, color, 0};

  if(semop(id, &operation, 1) == -1){
    return -1;
  }

  return id;
}

void private_close_traffic(int id){
  semctl(id, 0, IPC_RMID);
}

int create_pointer_traffic(CerradoSyn *self, const char *className, int initial_pointer){
  
  int result = -1;
  int number_of_traffics = 1;
  key_t key;

  if(initial_pointer <= 0){
    initial_pointer = RED_TRAFFIC;
  }
  if(initial_pointer > 0){
    initial_pointer = GREEN_TRAFFIC;
  }

  if((result = private_creat_a_wait_point(className, initial_pointer, number_of_traffics, &key)) == -1){
    return -1;
  }

  TrafficPointersList *objs_traffic = self->memory->traffic;
  objs_traffic->semID = (TrafficPointerObject **)realloc(objs_traffic->semID, sizeof(TrafficPointerObject *) * (objs_traffic->size_elements + 1));
  if(objs_traffic->semID == NULL){
    return -1;
  }

  TrafficPointerObject *obj_traffic = (TrafficPointerObject *)malloc(sizeof(TrafficPointerObject));
  if(obj_traffic == NULL){
    return -1;
  }

  obj_traffic->traffic_ID = result;
  obj_traffic->number_traffics = number_of_traffics;
  obj_traffic->key = key;
  obj_traffic->nameClass = className;

  objs_traffic->semID[objs_traffic->size_elements] = obj_traffic;

  objs_traffic->size_elements++;

  return 1;
}

int wait_traffic(MemoryShared *memory, const char *className, int color){

  key_t key = private_generate_string_key(className, memory->pid);
  if(!key){
    return -1;
  }

  if(private_wait(key, 1, 0, color) == -1){
    return -1;
  }

  return 1;
}

int signal_traffic(MemoryShared *memory, const char *className, int color){

  key_t key = private_generate_string_key(className, memory->pid);
  if(!key){
    return -1;
  }

  int sem_point = semget(key, 1, 0);
  if(sem_point == -1){
    return -1;
  }

  int result = -1;
  if((result = private_get_stats_traffic(sem_point, 0)) == -1){
    return -1;
  }

  if(color == GREEN_TRAFFIC){
    if(result <= 0){
      private_signal_traffic(sem_point, 0, 1);
      return 1;
    }
  }
  if(color == RED_TRAFFIC){
    if(result > 0){
      private_signal_traffic(sem_point, 0, -1);
      return 1;
    }
  }

  return 0;
}







//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end


int add_argument(CallbackProcess *callback_self, ArgumentCallback *add_arg){

  if(!callback_self || !add_arg || !callback_self->args){
    return -1;
  }

  ArgumentsCallback *self = callback_self->args;

  self->size_arguments++;

  self->arguments = (ArgumentCallback **)realloc(self->arguments, sizeof(ArgumentCallback *) * (self->size_arguments + 1));
  if(!self->arguments){
    self->size_arguments--;
    return -2;
  }

  self->arguments[self->size_arguments - 1] = (ArgumentCallback *)malloc(sizeof(*add_arg) + 1);
  if(!self->arguments[self->size_arguments - 1]){
    self->size_arguments--;
    return -3;
  }
  
  self->arguments[self->size_arguments - 1] = add_arg;

  return 1;
}


ArgumentCallback *new_argument(const char *name_argument, void *arg, size_t arg_size){

  if(!name_argument || !arg){
    return NULL;
  }

  ArgumentCallback *self = (ArgumentCallback *)malloc(sizeof(ArgumentCallback));
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->name_argument = (const char *)malloc(strlen(name_argument) + 1);
  if(!private_free_interrupted((char *)self->name_argument, (void *[]){self, NULL}, 2)){
    return NULL;
  }

  strcpy((char *)self->name_argument, name_argument);

  self->arg = (void *)malloc(arg_size + 1);
  if(!private_free_interrupted(self->arg, (void *[]){(char *)self->name_argument, self}, 2)){
    return NULL;
  }

  memcpy(self->arg, arg, arg_size + 1);

  return self;

}

void private_free_argument(ArgumentCallback *self){
  if(self != NULL){
    if(self->arg != NULL){
      free(self->arg);
    }
    if(self->name_argument != NULL){
      free((char *)self->name_argument);
    }
    free(self);
  }
}

ArgumentsCallback *private_new_ArgumentsCallback(){
  ArgumentsCallback *self = (ArgumentsCallback *)malloc(sizeof(ArgumentsCallback) + 1);
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->arguments = (ArgumentCallback **)malloc(sizeof(ArgumentCallback *) + 1);
  if(!private_free_interrupted(self->arguments, (void *[]){self}, 1)){
    return NULL;
  }

  self->arguments[0] = (ArgumentCallback *)malloc(0);
  if(!private_free_interrupted(self->arguments[0], (void *[]){self->arguments, self}, 2)){
    return NULL;
  }

  self->size_arguments = 0;

  return self;
}

void private_free_ArgumentsCallback(ArgumentsCallback *self){
  if(self){
  
    if(self->arguments){

      for(int i=0; i < self->size_arguments + 1; i++){
        if(self->arguments[i] != NULL){
          ArgumentCallback *argument = self->arguments[i];
          private_free_argument(argument);
        }
      }

      free(self->arguments);
    }

    free(self);
  }
}

CallbackProcess *new_CallbackProcess(CerradoSyn *process_father, int (*function)(MemoryShared *memory, ArgumentsCallback *arguments)){

  if(!function || !process_father){
    return NULL;
  }

  CallbackProcess *self = (CallbackProcess *)malloc(sizeof(CallbackProcess));
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->args = private_new_ArgumentsCallback();

  self->function_callback = function;

  self->memory = process_father->memory;

  return self;
}

void free_callback(CallbackProcess *self){
  if(self != NULL){

    private_free_ArgumentsCallback(self->args);
    
    free(self);
  }
}





//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end

CerradoSyn *new_CerradoSynStruct(const char *class_name, size_t size_max_memory_traffic){

  static bool primary_run = true;
  if(primary_run){
    setbuf(stdout, NULL);
  }
  primary_run = false;

  CerradoSyn *self = malloc(sizeof(CerradoSyn));
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->pid_father = getpid();

  self->process_list = (Process **)malloc(sizeof(Process *) * 2);
  if(!private_free_interrupted(self->process_list, (void *[]){self}, 1)){
    return NULL;
  }

  self->size_process = 0;

  self->name_class = (const char *)malloc(strlen(class_name) + 1);
  if(!private_free_interrupted((char *)self->name_class, (void *[]){self->process_list, self}, 2)){
    return NULL;
  }

  strcpy((char *)self->name_class, class_name);

  self->key = private_creat_key(self->name_class);
  if(!self->key){
    private_free_interrupted(NULL, (void *[]){(char *)self->name_class, self->process_list, self}, 3);
    return NULL;
  }

  self->class_list = NULL;
  
  self->memory = private_new_MemorySahred_struct(self->name_class, size_max_memory_traffic);
  if(!private_free_interrupted((MemoryShared *)self->memory, (void *[]){(char *)self->name_class, self->process_list, self}, 3)){
    return NULL;
  }

  return self;
}

void free_CerradoSyn(CerradoSyn *self){

  if(self != NULL){

    if(self->process_list != NULL){
      for(int i = 0; i < self->size_process + 1; i++){//O tamanho do process_list sempre vai ser uma unidade maior que o size_process por motivos de seguraça;
        if(self->process_list[i] != NULL){
          Process *process_temp = self->process_list[i];
          private_free_process(process_temp);
        }
      }
      free(self->process_list);
    }

    if(self->name_class != NULL){
      free((char *)self->name_class);
    }

    if(self->memory != NULL){
      private_delet_memory(self->memory);
    }

    free(self);
  }

  return;
}






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end


MemoryShared *private_new_MemorySahred_struct(const char *name_class, size_t size_max_traffic){
  
  short size_arguments = 0;

  MemoryShared *self = (MemoryShared *)malloc(sizeof(MemoryShared) + 1);
  if(!private_free_interrupted(self, NULL, size_arguments)){
    return NULL;
  }

  self->key = private_creat_key(name_class);
  if(!self->key){
    private_free_interrupted(NULL, (void *[]){self}, size_arguments);
    return NULL;
  }

  self->pid = getpid();

  self->memory_location = shmget(self->key, size_max_traffic, _CONFIG_SHMGET_PERMISSIONS_);
  if(self->memory_location == -1){
    private_free_interrupted(NULL, (void *[]){self}, ++size_arguments);
    printf("erroB");
    return NULL;
  }


  self->memory_shared = private_new_MemorySharedContent(self, size_max_traffic);
  if(!self->memory_shared){
    shmctl(self->memory_location, IPC_RMID, NULL);
    private_free_interrupted(NULL, (void *[]){self}, size_arguments);
    printf("erro");
    return NULL;
  }

  self->traffic = private_new_TrafficPointersList();
  if(!self->traffic){
    private_free_MemorySharedContent(self->memory_shared);
    shmctl(self->memory_location, IPC_RMID, NULL);
    private_free_interrupted(NULL, (void *[]){self}, size_arguments);
    printf("erro");
    return NULL;
  }

  return self;
}

void private_delet_memory(MemoryShared *memory_shared){
  if(memory_shared){
    private_free_TrafficPointersList(memory_shared->traffic);
    private_free_MemorySharedContent(memory_shared->memory_shared);

    shmctl(memory_shared->memory_location, IPC_RMID, NULL);

    free(memory_shared);
  }
}












//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



MemorySharedContent *private_new_MemorySharedContent(MemoryShared *memory_struct, size_t size_memoryShared){

  MemorySharedContent *self = (MemorySharedContent*)malloc(sizeof(MemorySharedContent));
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->size_memoryShared = size_memoryShared;

  self->traffic = private_new_TrafficMemory(memory_struct->key);
  if(!private_free_interrupted(self->traffic, (void *[]){self}, 1)){
    return NULL;
  }

  self->memoryShared = (void *)shmat(memory_struct->memory_location, NULL, 0);;
  if(self->memoryShared == (void *)-1){
    private_free_traffic(self->traffic);
    private_free_interrupted(NULL, (void *[]){self}, 1);
    return NULL;
  }

  self->size_memory = sizeof(self->memoryShared);
  self->memory = (void *)malloc(self->size_memory + 1);
  if(!self->memory){
    private_close_memory(memory_struct);
    private_free_traffic(self->traffic);
    private_free_interrupted(NULL, (void *[]){self}, 1);
    return NULL;
  }

  memcpy(self->memory, self->memoryShared, sizeof(self->memoryShared));

  return self;
}

void private_free_MemorySharedContent(MemorySharedContent *self){
  if(self){
    
    if(self->memory){
      free(self->memory);
    }
    
    if(self->memoryShared != (void *)-1){
      shmdt(self->memoryShared);
    }

    if(self->traffic){
      private_free_traffic(self->traffic);
    }

    free(self);
  }
}





//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



TrafficMemory *private_new_TrafficMemory(key_t key){
  
  TrafficMemory *self = (TrafficMemory *)malloc(sizeof(TrafficMemory));
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->key = key;
  self->trafficID = private_init_traffic(self->key);
  if(self->trafficID == -1){
    free(self);
    return NULL;
  }

  return self;
}

TrafficPointerObject *private_new_TrafficPointerObject(const char *className, int contTraffics, int initialPointer){

  if(!private_free_interrupted((void *)className, NULL, 0)){
    return NULL;
  }

  TrafficPointerObject *self = (TrafficPointerObject *)malloc(sizeof(TrafficPointerObject) + 1);
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->nameClass = (const char *)malloc(strlen(className) + 1);
  if(!private_free_interrupted((void *)self->nameClass, (void *[]){self}, 1)){
    return NULL;
  }

  self->nameClass = className;
  

  self->number_traffics = contTraffics;
  if(self->number_traffics < 1){
    private_free_interrupted(NULL, (void *[]){(void *)self->nameClass, self}, 2);
    return NULL;
  }

  if((self->traffic_ID = private_creat_a_wait_point(self->nameClass, initialPointer, self->number_traffics, &self->key)) == -1){
    private_free_interrupted(NULL, (void *[]){(void *)self->nameClass, self}, 2);
    return NULL;
  }

  return self;

}

TrafficPointersList *private_new_TrafficPointersList(){

  TrafficPointersList *self = (TrafficPointersList *)malloc(sizeof(TrafficPointersList) + 1);
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->size_elements = 0;
  self->semID = (TrafficPointerObject **)malloc(sizeof(TrafficPointerObject *));
  if(!private_free_interrupted(self->semID, (void *[]){self}, 1)){
    return NULL;
  }

  return self;
}

void private_free_traffic(TrafficMemory *self){
  if(self){
    private_close_traffic(self->trafficID);
    free(self);
  }
}

void private_free_TrafficPointerObject(TrafficPointerObject *self){
  if(self){

    if(self->traffic_ID != -1){
      private_close_traffic(self->traffic_ID);
    }

    free(self);
  }
}

void private_free_TrafficPointersList(TrafficPointersList *self){

  if(self){

    if(self->semID){
      for(int i = 0; i < self->size_elements; i++){
        private_free_TrafficPointerObject(self->semID[i]);
      }
      free(self->semID);
    }
    
    free(self);
  }
}







//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



Process *private_new_process(int size_stack){

  Process *self = (Process *)malloc(sizeof(Process));
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->size_stack = size_stack;
  self->stack = malloc(self->size_stack);
  if(!private_free_interrupted(self->stack, (void *[]){self}, 1)){
    return NULL;
  }

  return self;
}


void private_free_process(Process *self){

  if(self != NULL){
  
    if(self->stack != NULL){
      free(self->stack);
    }
    free(self);

  }
  
  return;
}




#endif
