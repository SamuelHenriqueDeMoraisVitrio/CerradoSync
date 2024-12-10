#ifndef silverchain_imp
#define silverchain_imp

#define _GNU_SOURCE
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>






#endif

#ifndef silverchain_depG
#define silverchain_depG

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

#ifndef silverchain_depH
#define silverchain_depH

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

#ifndef silverchain_mac
#define silverchain_mac

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



#define _SIZE_STACK_PROCESS_1MB_ 1024 * 1024




#endif

#ifndef silverchain_typesE
#define silverchain_typesE

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end


typedef struct Sha_256 Hash;




//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end

typedef struct ProcessStruct Process;

typedef struct CerradoSynStruct CerradoSyn;

typedef struct Arguments_struct ArgumentsCallback;

typedef struct Argument_struct ArgumentCallback;

typedef struct CallbackProcess_struct CallbackProcess;



#endif

#ifndef silverchain_typesF
#define silverchain_typesF

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

#ifndef silverchain_typesG
#define silverchain_typesG

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end





struct CallbackProcess_struct{
  int(*function_callback)(ArgumentsCallback *arguments);
  ArgumentsCallback *args;
};





//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end


struct ProcessStruct{
  pid_t process;
  void *stack;
  int size_stack;
};







#endif

#ifndef silverchain_typesH
#define silverchain_typesH

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end


struct CerradoSynStruct{
  int pid_father;
  const char *name_class;

  Process **process_list;
  size_t size_process;
  
  void *memory;
  CerradoSyn *class_list;
};







#endif

#ifndef silverchain_dec
#define silverchain_dec

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end


int private_free_interrupted(void *arg_for_verify, void **args_for_free, size_t size_args);






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



const char *private_generate_string_key(const char *key, pid_t hierarchy);

const char *private_creat_key(const char *key);

const char *private_get_key(const char *key);





//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



void private_clone_process(Process *process, CallbackProcess *callback, int *flags);





//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end


bool create_process(CerradoSyn *main_process, CallbackProcess *callback, int *flags);




//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



CallbackProcess *new_CallbackProcess(int (*function)(ArgumentsCallback *arguments), ArgumentCallback *primary_arg);

ArgumentCallback *new_argument(const char *name_argument, void *arg, size_t arg_size);

void private_free_argument(ArgumentCallback *self);

void free_callback(CallbackProcess *self);

void add_argument(CallbackProcess *callback_self, ArgumentCallback *add_arg);







//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end




CerradoSyn *new_CerradoSynStruct(const char *class_name);

void free_CerradoSyn(CerradoSyn *self);






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



Process *private_new_process(int size_stack);

void private_free_process(Process *self);





#endif

#ifndef silverchain_def
#define silverchain_def

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

  return -1;
}






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



const char *private_generate_string_key(const char *key, pid_t hierarchy){
  if(key == NULL){
    perror("\n\tKey passada incorreta;\n");
    return 0;
  }

  short size_pid = 15;
  long size_key_formated = strlen(key) + size_pid + 3;

  char key_hash[size_key_formated];
  snprintf(key_hash, size_key_formated, "%s-%d", key, hierarchy);

  uint8_t hash[SIZE_OF_SHA_256_HASH];
  calc_sha_256(hash, key_hash, strlen(key_hash));

  char *hash_string = (char*)malloc(SIZE_OF_SHA_256_HASH * 2 + 1);
  for (unsigned int i = 0; i < SIZE_OF_SHA_256_HASH; i++) {
      sprintf(hash_string + i * 2, "%02x", hash[i]);
  }

  return hash_string;
}

const char *private_creat_key(const char *key){
  return private_generate_string_key(key, getpid());
}

const char *private_get_key(const char *key){
  return private_generate_string_key(key, getppid());
}







//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end

int callback_config(void *arg){


  CallbackProcess *stuct_arg = (CallbackProcess *)arg;

  int(*function_callback)(ArgumentsCallback *arguments);
  function_callback = stuct_arg->function_callback;

  int var_return = function_callback(stuct_arg->args);

  return var_return;
}

void private_clone_process(Process *process, CallbackProcess *callback, int *flags){

  if(!process){
    perror("Error cloning process");
    exit(EXIT_FAILURE);
  }

  pid_t pid_process = clone(callback_config, process->stack + process->size_stack - 1, flags?*flags:SIGCHLD, callback);

  process->process = pid_process;


}






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end




bool create_process(CerradoSyn *main_process, CallbackProcess *callback, int *flags) {
  if (getpid() != main_process->pid_father) {
      return false;
  }

  if(!main_process || !callback){
    return false;
  }

  Process *new_process = private_new_process(_SIZE_STACK_PROCESS_1MB_);

  private_clone_process(new_process, callback, flags);
  if(new_process->process == -1){
    private_free_process(new_process);
    return false;
  }

  main_process->size_process++;

  main_process->process_list = (Process **)realloc(main_process->process_list, (main_process->size_process + 1) * sizeof(Process *));

  main_process->process_list[main_process->size_process - 1] = new_process;

  return true;
}






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end


void add_argument(CallbackProcess *callback_self, ArgumentCallback *add_arg){

  if(!callback_self || !add_arg){
    return;
  }

  callback_self->args->size_arguments++;

  callback_self->args->arguments = (ArgumentCallback **)realloc(callback_self->args->arguments, callback_self->args->size_arguments + 1);
  if(callback_self->args->arguments == NULL){
    callback_self->args->size_arguments--;
    return;
  }

  callback_self->args->arguments[callback_self->args->size_arguments - 1] = (ArgumentCallback *)malloc(sizeof(*add_arg) + 1);
  if(callback_self->args->arguments == NULL){
    callback_self->args->size_arguments--;
    return;
  }
  
  callback_self->args->arguments[callback_self->args->size_arguments - 1] = add_arg;
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

  memcpy(self->arg, arg, arg_size);

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

CallbackProcess *new_CallbackProcess(int (*function)(ArgumentsCallback *arguments), ArgumentCallback *primary_arg){

  if(!function || !primary_arg){
    return NULL;
  }

  CallbackProcess *self = (CallbackProcess *)malloc(sizeof(CallbackProcess));
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->function_callback = function;

  self->args = (ArgumentsCallback *)malloc(sizeof(ArgumentsCallback) * 2);
  if(!private_free_interrupted(self->args, (void *[]){self}, 1)){
    return NULL;
  }

  self->args->arguments = (ArgumentCallback **)malloc(sizeof(ArgumentCallback *) * 2);
  if(!private_free_interrupted(self->args->arguments, (void *[]){self->args, self}, 2)){
    return NULL;
  }

  self->args->size_arguments = 1;

  self->args->arguments[0] = (ArgumentCallback *)malloc(sizeof(*primary_arg));
  if(!private_free_interrupted(self->args->arguments[0], (void *[]){self->args->arguments, self->args, self}, 3)){
    return NULL;
  }

  self->args->arguments[0] = primary_arg;

  return self;

}

void free_callback(CallbackProcess *self){
  if(self != NULL){

    if(self->args != NULL){

      if(self->args->arguments != NULL){

        for(int i=0; i < self->args->size_arguments + 1; i++){
          if(self->args->arguments[i] != NULL){
            ArgumentCallback *argument = self->args->arguments[i];
            private_free_argument(argument);
          }
        }
        
        free(self->args->arguments);
      }
      free(self->args);
    }
    free(self);
  }
}





//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end

CerradoSyn *new_CerradoSynStruct(const char *class_name){

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

  self->class_list = NULL;
  
  self->memory = NULL;

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

    free(self);
  }

  return;
}






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



Process *private_new_process(int size_stack){

  Process *self = malloc(sizeof(Process));
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
