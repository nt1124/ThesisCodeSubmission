University of Bristol - MEng. Dissertation
Mathematics and Computer Science.

Author : Nicholas James Tutte - nt1124
Supervisor : Prof. Nigel Smart

Title : Secure Two Party Computation - A practical comparison of recent protocols.


*****************************

	=== Build Command ===

	g++ circuitEvaluator.c -O3 -fopenmp -ffast-math -maes -lgmp -lrt -o a.out

	=== Run Command ===

	./a.out [Circuit] [IP] [PortNum] [PartyInput] [ProtocolNum] [PartyID]

*****************************


Here we give a short summary of each folder/file included in our implementation in alphabetic order.
+ indicates a folder.
- indicates a file.

+ Root
	- circuitEvaluator.c : The main source code file.


	+ circuitFiles : Contains the input files representing the circuits to be evaluated


	+ Benchmarking : Folder containing source code for benchmarking the implementation on some key metrics, e.g. communications, EC group operations.

		- benchmarking.(c/h) : Source code binding all the benchmarking together.

		- benchmarkingCircuits.c : Source code for benchmarking the building and execution of circuits.

		- benchmarkingComms.c : Source code for benchmarking the cummunication of raw bytes and group elements.

		- benchmarkingECC.c : Source code for benchmarking the Elliptic curve group operation.

		- benchmarkingOTs.c : Source code for benchmarking the Oblivious Transfers 



	+ circuits : Folder containing source code relating to circuits, also contains the protocol implementation code.

		- circuitUtils.h : Header binding all the circuit/circuit building/crypto code together.

		- gateOrWire.(c/h) : Source code for the gates/wires of a Yao Garbled Circuit. Also has the data structure for a circuit made from these gates/wires.

		- circuitUtils.c : Source code for utility functions, e.g. Printing output of circuit.

		- circuitCommunications.c : Source code for sending and receiving circuits.

		- circuitCorrectnessChecks : Source code for checking if two circuits are the same, used for checking correctness.


		+ LP_2010 : Folder containing all the source code directly relating to the Lindell-Pinkas 2010 implementation

			- LP_S2C_CnC_OT.h : Header file binding all the Lindell-Pinkas 2010 source code together.

			- LP_S2C_CnC_OT_Builder_ECC.c : Source code file, mostly utils relating to the builder. For example code to run all the OTs required, and to build the circuits.

			- LP_S2C_CnC_OT_Executor_ECC.c : Source code file, mostly utils relating to the Executor.

			- LP_S2C_CnC_OT.c : Source code file, main functions for Builder and Executor in the LP-2010 protocol.


		+ L_2013 : Folder containing all the source code directly relating to the implementation of the  Lindell 2013 protocol

			- L_CnC_OT.h : Header file binding all the Lindell 2013 source code together.

			- L_CnC_OT_Check_Builder.c : Source code file, functions for running the sub-computation for the Builder.

			- L_CnC_OT_ECC_Builder.c : Source code file, mostly utils relating to the main computation for the Builder. For example code to run all the OTs required, and to build the circuits. Calls the main of the L_CnC_OT_Check_Builder.c.

			- L_CnC_OT_Check_Executor.c : Source code file, functions for running the sub-computation for the Executor.

			- L_CnC_OT_ECC_Executor.c : Source code file, mostly utils relating to the Executor. Calls the main of the L_CnC_OT_Check_Executor.c.

			- L_CnC_OT.c : Source code file, main functions for Builder and Executor in the L-2013 protocol.


		+ HKE_2013 : Folder containing all the source code directly relating to the implementation of the Huang-Katz-Evans 2013 protocol.

			- HKE_2013.h : Header binding it alltogether.

			- HKE_2013_Commitments.c : Source code for the commitments of the party's inputs to their own circuis.

			- HKE_2013_OT.c : Source code for running all the OT functionality for the HKE protocol using the Naor-Pinkas OT.

			- HKE_2013_Utils.c : Source code for some utilites for the HKE protocol, notably functions to serialise and deserialise J-set openings.

			- HKE_2013_Output_Determination.c : Source code for output determination of the HKE protocol using the secret sharing schemes.

			- HKE_2013.c : Source code for main parts of HKE protocol.



		+ L_2013_HKE : Folder containing all the source code directly relating to the implementation of the modification of the Lindell 2013 protcol to use the Huang-Katz-Evans protocol for the sub-computation.

			- L_2013_HKE.h : Header file binding all the modified Lindell 2013 source code together.

			- L_2013_HKE_Check_Builder.c : Source code file, functions for running the sub-computation for the Builder. Uses the modified sub-computation using HKE.

			- L_2013_HKE_ECC_Builder.c : Source code file, mostly utils relating to the main computation for the Builder. For example code to run all the OTs required, and to build the circuits. Calls the main of the L_2013_HKE_Check_Builder.c.

			- L_2013_HKE_Check_Executor.c : Source code file, functions for running the sub-computation for the Executor. Uses the modified sub-computation using HKE.

			- L_2013_HKE_ECC_Executor.c : Source code file, mostly utils relating to the Executor. Calls the main of the L_2013_HKE_Check_Executor.c.

			- L_2013_HKE.c : Source code file, main functions for Builder and Executor in the L-2013 protocol.


	+ comms : Folder containing the source code for the raw communication between parties. Mostly coded by Prof. Nigel Smart, we have added in measurement code to track how many bytes are being sent. In future we need to alter to use long ints etc for 


	+ crypto : Folder containing source code relating to cryptographic primitives (e.g. OTs, AES etc).

		- cryptoUtil.(c/h) : Utils source code, binds all the crypto together, 

		- gmpUtils.c : Utilities for GNU Multi-precision arithmetic library. In particular MPZ serialisation.

		- randUtils.c : Utilities generating .

		- aes.(cpp/h) :  AES source code, mostly written by Dr. Dan Page, some addition work done by me.

		- DDH_Primitive.(c/h) : DDH based cryptosystem primitive source code.


		+ OT : Folder containing all source code for the Oblivious Transfer primitives.

			+ otL_Mod_CnC : Folder containing the code for the primitives underlying the Modified Cut and Choose OT.

			+ otLP_CnC : Folder containing the code for the primitives underlying the original Cut and Choose OT from before we switched to ECC over Schnorr groups. Mostly obsolete code but some utility code still used. In future should be pruned and put into otLP_ECC_CnC.

			+ otLP_ECC_CnC : Folder containing the code for the primitives underlying the original Cut and Choose OT afterthe switch from Schnorr groups to ECC.

			+ otNaorPinkas : Folder containing the code for the primitives underlying the Naor-Pinkas Oblivious Transfer.

			+ otPVW : Folder containing the code for the primitives underlying the PVW OT based on the Dual-mode cryptosystem from before the switch to ECC. Mostly obsolete code but some utility code still used. In future should be pruned.

			+ otPVW_ECC : Folder containing the code for the primitives underlying the PVW OT, the ECC version.


		+ SecretSharing : Folder containing all source code for Secret scharing schemes (including F_q Polynomials and lagrange interpolation).

			- gmp_poly.(c/h) : Code for the multi-precision polynomials.

			- lagrangeInterpolation.(c/h) : Code for running Lagrange Interpolation, also will share .

			- verifiedSecretSharing.(c/h) : Code for the Feldman Secret Sharing Scheme. Uses code from both other code file pairs in the folder.

		+ CommitmentSchemes : Folder containing the code for the commitment scheme used in the HKE setting. Based on the Pedersen-Hash commitment.

		+ ConsistencyChecks : Folder containing the code for the Extended DH tuple based consistency, these files only deal with generating the inputs, proving is in the Zero knowedlge proofs area.

		+ EllipticCurves : Folder containing the Elliptic curve groups etc. Akin to the DDH_primitives files but as we need to make the group at well more code needed.

		+ Hashing : Folder containing hashing code (just SHA-256 for now)

		+ ISAAC : Folder containing the ISAAC code taken from http://burtleburtle.net/bob/rand/isaacafa.html

		+ SecureCoinToss : Folder containing source code for a secure coin toss in the ROM model.

		+ ZeroKnowledgeProofs : Folder containing source code for Zero Knowledge proofs.



	+ fileUtils : Folder containing the source code for reading input/circuit files, but also for converting a binary circuit read from file to a Yao Garbled Circuit.

		- createRawCheckCircuit.(c/h) : Source code to create a circuit for the sub-computation. Contains functions for both the circuit with all Lindell's optimisations and the rolled back and output hiding circuit used for the L-HKE protocol.

		- fileUtils_FromRaw.(c/h) : Code to take a raw circuit and build a Yao Garbled circuit for ASYMMETRIC protocols.

		- fileUtils_FromRaw_HKE_2013.(c/h) : Code to take a raw circuit and build a Yao Garbled circuit for SYMMETRIC protocols.

		- fileUtilsRTL.(c/h) : Code to create a Yao Circuit directly from the circuit file.

		- inputFileUtils.(c/h) : Code to read in party computation input files.

		- readRawCircuit.(c/h) : Code to read in a circuit definition file and create a Raw binary circuit from this file.


	+ inputs : Folder contain the input files representing the input of the parties to the computations.





