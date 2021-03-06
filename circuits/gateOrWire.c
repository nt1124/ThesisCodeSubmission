// Prints a gateOrWire structure, used in debugging.
void printGateOrWire(struct gateOrWire *inputGW)
{
	int i, j;
	unsigned char *temp;

	printf("G_ID                    =  %d\n", inputGW -> G_ID);
	printf("Wire Mask               =  %02X\n", inputGW -> outputWire -> wireMask);
	printf("Wire Owner              =  %02X\n", inputGW -> outputWire -> wireOwner);
	printf("Wire Perm               =  %02X\n", (0x01 & inputGW -> outputWire -> wirePerm));
	printf("Wire Permed Value       =  %02X\n", inputGW -> outputWire -> wirePermedValue);
	printf("Wire True Value         =  %02X\n", (0x01 & inputGW -> outputWire -> wirePerm) ^ inputGW -> outputWire -> wirePermedValue);
	printf("Out key                 =  ");
	for(i = 0; i < 16; i ++)
		printf("%02X", inputGW -> outputWire -> wireOutputKey[i]);
	printf("\n");
	fflush(stdout);


	if(0x00 == inputGW -> outputWire -> wireOwner &&
	   0x01 == (0x0F & inputGW -> outputWire -> wireMask) )
	{
		printf("key0                    =  ");
		for(i = 0; i < 16; i ++)
			printf("%02X", inputGW -> outputWire -> outputGarbleKeys -> key0[i]);
		printf("\n");
		fflush(stdout);

		printf("key1                    =  ");
		for(i = 0; i < 16; i ++)
			printf("%02X", inputGW -> outputWire -> outputGarbleKeys -> key1[i]);
		printf("\n");
		fflush(stdout);
	}

	if(NULL != inputGW -> gatePayload)
	{
		printf("numInputs               =  %d\n", inputGW -> gatePayload -> numInputs);
		printf("outputTableSize         =  %d\n", inputGW -> gatePayload -> outputTableSize);
		fflush(stdout);

		for(i = 0; i < inputGW -> gatePayload -> numInputs; i ++)
			printf("inputIDs[%d]             =  %d\n", i, inputGW -> gatePayload -> inputIDs[i]);
		fflush(stdout);

		temp = (unsigned char*) calloc(32, sizeof(unsigned char));
		for(i = 0; i < inputGW -> gatePayload -> outputTableSize; i ++)
		{
			printf("encOutputTable[%d]       =  ", i);
			memcpy(temp, inputGW -> gatePayload -> encOutputTable[i], 32);
			for(j = 0; j < 32; j ++)
			{
				printf("%02X", temp[j]);
			}
			printf("\n");
		}
		fflush(stdout);
	}
}


// Callocs the memory for the encrypted output table.
unsigned char **createOutputTable(struct gate *curGate)
{
	unsigned char **toReturn = (unsigned char**) calloc(curGate -> outputTableSize, sizeof(unsigned char*));
	int i;

	for(i = 0; i < curGate -> outputTableSize; i ++)
	{
		toReturn[i] = (unsigned char*) calloc(32, sizeof(unsigned char));
		toReturn[i][16] = curGate -> rawOutputTable[i];
	}

	return toReturn;
}


// Encrypt the whole of the output table for a given gate.
void encWholeOutTable(struct gateOrWire *curGate, struct gateOrWire **circuit)
{
	int numInputs = curGate -> gatePayload -> numInputs;
	int i, j, tempBit, permedIndex, inputID;
	struct wire *inputWire;
	unsigned char *keyList[numInputs], *tempRow;
	unsigned char *toEncrypt0 = (unsigned char*) calloc(32, sizeof(unsigned char));
	unsigned char *toEncrypt1 = (unsigned char*) calloc(32, sizeof(unsigned char));
	
	memcpy(toEncrypt0, curGate -> outputWire -> outputGarbleKeys -> key0, 17);
	memcpy(toEncrypt1, curGate -> outputWire -> outputGarbleKeys -> key1, 17);

	// For each input wire, calloc space for a 128-bit key.
	for(j = 0; j < numInputs; j ++)
	{
		keyList[j] = (unsigned char*) calloc(16, sizeof(unsigned char));
	}

	// For each output table entry
	for(i = 0; i < curGate -> gatePayload -> outputTableSize; i ++)
	{
		permedIndex = 0;
		// For each input bit for this outputTable entry.
		for(j = 0; j < numInputs; j ++)
		{
			// Get the ID of the input wire for the current bit.
			inputID = curGate -> gatePayload -> inputIDs[j];
			inputWire = circuit[inputID] -> outputWire;

			// Get the value of this wire for this bit of this input.
			tempBit = (i >> j) & 0x01;
			permedIndex ^= (inputWire -> wirePerm & 0x01) << j;

			// Copy the key for this bit in this input into the key list.
			if(0 == tempBit)
				memcpy(keyList[j], inputWire -> outputGarbleKeys -> key0, 16);
			else if(1 == tempBit)
				memcpy(keyList[j], inputWire -> outputGarbleKeys -> key1, 16);
		}
		// Permuted index.
		permedIndex = i ^ permedIndex;

		if(0 == curGate -> gatePayload -> rawOutputTable[i])
			tempRow = encryptMultipleKeys(keyList, numInputs, toEncrypt0, 2);
		else if(1 == curGate -> gatePayload -> rawOutputTable[i])
			tempRow = encryptMultipleKeys(keyList, numInputs, toEncrypt1, 2);

		// Copy the encrypted output to the encOutputTable. All 32 bytes of it.
		memcpy(curGate -> gatePayload -> encOutputTable[permedIndex], tempRow, 32);
		free(tempRow);
	}

	// Tidy up temporary memory we were using.
	for(j = 0; j < numInputs; j ++)
	{
		free(keyList[j]);
	}
	free(toEncrypt0);
	free(toEncrypt1);
}


// Decrypt the gate using it's inputs, eventually should only be used for the non-XOR gates.
void decryptGate(struct gateOrWire *curGate, struct gateOrWire **inputCircuit)
{
	int numInputs = curGate -> gatePayload -> numInputs;
	int j, tempBit, tempIndex, outputIndex = 0, h;
	unsigned char *keyList[numInputs], *toReturn;
	unsigned char *tempRow;


	// For the all the input wires to this gate.
	for(j = 0; j < numInputs; j ++)
	{
		// Get the index of the j^th input wire, then get that wires permutated output value.
		tempIndex = curGate -> gatePayload -> inputIDs[numInputs - j - 1];
		tempBit = inputCircuit[tempIndex] -> outputWire -> wirePermedValue;

		// Get the next bit of the outputIndex.
		outputIndex = (outputIndex << 1) + tempBit;

		// Copy into our key list
		keyList[j] = (unsigned char*) calloc(16, sizeof(unsigned char));
		memcpy(keyList[j], inputCircuit[tempIndex] -> outputWire -> wireOutputKey, 16);
	}

	// Get the row of the output table indexed by the values of our input wires.
	tempRow = curGate -> gatePayload -> encOutputTable[outputIndex];


	// Then decrypt this row using the key list, copy first 16 bits into the output key field.
	toReturn = decryptMultipleKeys(keyList, numInputs, tempRow, 2);
	memcpy(curGate -> outputWire -> wireOutputKey, toReturn, 16);

	// Get the decrypted permutated wire value.
	curGate -> outputWire -> wirePermedValue = toReturn[16];
	
	// Housekeeping.
	for(j = 0; j < numInputs; j ++)
	{
		free(keyList[j]);
	}
	free(toReturn);
}


// Evaluate a gate using the Free-XOR methods.
void freeXOR_Gate(struct gateOrWire *curGate, struct gateOrWire **inputCircuit)
{
	int numInputs = curGate -> gatePayload -> numInputs;
	int i, j, tempIndex, outputIndex = 0;
	unsigned char *toReturn = (unsigned char *) calloc(16, sizeof(unsigned char));
	unsigned char outputBit = 0x00;

	// For the all the input bits 
	for(i = 0; i < numInputs; i ++)
	{
		// Get the index of the i^th input wire, then get that wires permutated output value.
		tempIndex = curGate -> gatePayload -> inputIDs[numInputs - i - 1];
		outputBit ^= inputCircuit[tempIndex] -> outputWire -> wirePermedValue;

		for(j = 0; j < 16; j ++)
		{
			toReturn[j] ^= inputCircuit[tempIndex] -> outputWire -> wireOutputKey[j];
		}
	}

	// Get the decrypted permutated wire value.
	curGate -> outputWire -> wirePermedValue = outputBit;

	// Housekeeping.
	memcpy(curGate -> outputWire -> wireOutputKey, toReturn, 16);
	free(toReturn);
}


// Are we dealing with an XOR gate.
void evaulateGate(struct gateOrWire *curGate, struct gateOrWire **inputCircuit)
{
	unsigned char temp;


	// Check whether the gate can be Free-XORed or not.
	if( 0xF0 != (0xF0 & curGate -> outputWire -> wireMask) || 0x02 == curGate -> outputWire -> wireMask )
	{
		decryptGate(curGate, inputCircuit);
	}
	else
	{
		freeXOR_Gate(curGate, inputCircuit);
	}
}


// Generate two RANDOM garble keys. This should not be used when doing the Free-XOR as
// there is no relation between the two keys.
struct bitsGarbleKeys *generateGarbleKeyPair(unsigned char perm)
{
	struct bitsGarbleKeys *toReturn = (struct bitsGarbleKeys*) calloc(1, sizeof(struct bitsGarbleKeys));

	toReturn -> key0 = generateRandBytes(16, 17);
	toReturn -> key0[16] = 0x00 ^ (0x01 & perm);
	
	toReturn -> key1 = generateRandBytes(16, 17);
	toReturn -> key1[16] = 0x01 ^ (0x01 & perm);

	return toReturn;
}


// Generate one random garble key, create the other using this first one and a global random called R.
// This is the essence of the Free-XOR optimisation.
struct bitsGarbleKeys *genFreeXORPairInput(unsigned char perm, unsigned char *R)
{
	struct bitsGarbleKeys *toReturn = (struct bitsGarbleKeys*) calloc(1, sizeof(struct bitsGarbleKeys));
	int i;

	toReturn -> key0 = generateRandBytes(17, 17);
	toReturn -> key0[16] = 0x00 ^ (0x01 & perm);
	
	toReturn -> key1 = (unsigned char*) calloc(17, sizeof(unsigned char));

	for(i = 0; i < 17; i ++)
	{
		toReturn -> key1[i] = toReturn -> key0[i] ^ R[i];
	}
	toReturn -> key1[16] = 0x01 ^ (0x01 & perm);


	return toReturn;
}


// Generate one random garble keys, create the other using this first one and a global random called R.
// This is the essence of the Free-XOR optimisation.
struct bitsGarbleKeys *genFreeXORPairInput(randctx *ctx, unsigned char perm, unsigned char *R)
{
	struct bitsGarbleKeys *toReturn = (struct bitsGarbleKeys*) calloc(1, sizeof(struct bitsGarbleKeys));
	int i;

	toReturn -> key0 = generateIsaacRandBytes(ctx, 17, 17);
	toReturn -> key0[16] = 0x00 ^ (0x01 & perm);
	
	toReturn -> key1 = (unsigned char*) calloc(17, sizeof(unsigned char));

	for(i = 0; i < 17; i ++)
	{
		toReturn -> key1[i] = toReturn -> key0[i] ^ R[i];
	}
	toReturn -> key1[16] = 0x01 ^ (0x01 & perm);


	return toReturn;
}


// Slightly different from the case of the Input wire as we need to use the XORing of all input wires key0.
// Basically no random here.
struct bitsGarbleKeys *genFreeXORPair(struct gateOrWire *curGate, unsigned char *R, struct gateOrWire **circuit)
{
	struct bitsGarbleKeys *toReturn = (struct bitsGarbleKeys*) calloc(1, sizeof(struct bitsGarbleKeys));
	int i, j, tempIndex;

	toReturn -> key0 = (unsigned char*) calloc(17, sizeof(unsigned char));


	// Get key0, using the key0 of all input wires to this gate XORed.
	for(i = 0; i < curGate -> gatePayload -> numInputs; i ++)
	{
		// Get the i^th input wire.
		tempIndex = curGate -> gatePayload -> inputIDs[i];

		// XOR the 16 bytes.
		for(j = 0; j < 16; j ++)
		{
			toReturn -> key0[j] ^= circuit[tempIndex] -> outputWire -> outputGarbleKeys -> key0[j];
		}
	}
	
	// Permutation added onto the 
	toReturn -> key0[16] = 0x00 ^ (0x01 & curGate -> outputWire -> wirePerm);

	// XOR the key with R
	toReturn -> key1 = (unsigned char*) calloc(17, sizeof(unsigned char));
	for(i = 0; i < 16; i ++)
	{
		toReturn -> key1[i] = toReturn -> key0[i] ^ R[i];
	}
	toReturn -> key1[16] = 0x01 ^ (0x01 & curGate -> outputWire -> wirePerm);


	return toReturn;
}


// Get a random permutation. Well really we get 8 of them but hey ho only use 1.
unsigned char getPermutation()
{
	unsigned char *toOutputPointer = generateRandBytes(1, 1);
	unsigned char toReturn = (*toOutputPointer) & 0x01;
	free(toOutputPointer);

	return toReturn;
}


// Get a random permutation. Well really we get 8 of them but hey ho only use 1.
unsigned char getIsaacPermutation(randctx *ctx)
{
	unsigned char *toOutputPointer = generateIsaacRandBytes(ctx, 1, 1);
	unsigned char toReturn = (*toOutputPointer) & 0x01;
	free(toOutputPointer);

	return toReturn;
}


// Function to encrypt all the output tables.
void garbleOutputTables(struct Circuit *inputCircuit)
{
	int i;

	// For each gate, if not gatePayload.
	for(i = 0; i < inputCircuit -> numGates; i ++)
	{
		if(NULL != inputCircuit -> gates[i] -> gatePayload)
		{
			encWholeOutTable(inputCircuit -> gates[i], inputCircuit -> gates);
		}
	}
}


// Ronseal.
void freeGate(struct gate *inputGate)
{
	int i;

	if(NULL != inputGate)
	{
		free(inputGate -> inputIDs);

		for(i = 0; i < inputGate -> outputTableSize; i ++)
		{
			free(inputGate -> encOutputTable[i]);
			inputGate -> encOutputTable[i] = NULL;
		}

		if(NULL != inputGate)
		{
			free(inputGate -> encOutputTable);
			inputGate -> encOutputTable = NULL;
		}

		if(NULL != inputGate -> rawOutputTable)
			free(inputGate -> rawOutputTable);
		
		free(inputGate);
	}
}


// Ronseal.
void freeGateOrWire(struct gateOrWire *inputGW)
{
	freeGate(inputGW -> gatePayload);
	
	if(NULL != inputGW -> outputWire -> outputGarbleKeys)
	{
		free(inputGW -> outputWire -> outputGarbleKeys -> key0);
		free(inputGW -> outputWire -> outputGarbleKeys -> key1);
		free(inputGW -> outputWire -> outputGarbleKeys);
	}

	if(NULL != inputGW -> outputWire)
	{
		if(NULL != inputGW -> outputWire -> wireOutputKey)
			free(inputGW -> outputWire -> wireOutputKey);

		free(inputGW -> outputWire);
	}

	free(inputGW);
}
