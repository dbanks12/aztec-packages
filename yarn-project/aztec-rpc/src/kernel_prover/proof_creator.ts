import {
  CircuitError,
  CircuitsWasm,
  KernelCircuitPublicInputs,
  KernelCircuitPublicInputsFinal,
  PrivateCircuitPublicInputs,
  PrivateKernelInputsInit,
  PrivateKernelInputsInner,
  PrivateKernelInputsOrdering,
  Proof,
  makeEmptyProof,
  privateKernelSimInit,
  privateKernelSimInner,
  privateKernelSimOrdering,
} from '@aztec/circuits.js';
import { siloNoteHash } from '@aztec/circuits.js/abis';
import { Fr } from '@aztec/foundation/fields';
import { createDebugLogger } from '@aztec/foundation/log';

/**
 * Represents the output of the proof creation process for init and inner private kernel circuit.
 * Contains the public inputs required for the init and inner private kernel circuit and the generated proof.
 */
export interface ProofOutput {
  /**
   * The public inputs required for the proof generation process.
   * Note: C++ side does not define the specific data structure PrivateKernelPublicInputs and therefore
   * would not generate a binding in circuits.gen.ts.
   */
  publicInputs: KernelCircuitPublicInputs;
  /**
   * The zk-SNARK proof for the kernel execution.
   */
  proof: Proof;
}

/**
 * Represents the output of the proof creation process for final ordering private kernel circuit.
 * Contains the public inputs required for the final ordering private kernel circuit and the generated proof.
 */
export interface ProofOutputFinal {
  /**
   * The public inputs required for the proof generation process.
   * Note: C++ side does not define the specific data structure PrivateKernelPublicInputsFinal and therefore
   * would not generate a binding in circuits.gen.ts.
   */
  publicInputs: KernelCircuitPublicInputsFinal;
  /**
   * The zk-SNARK proof for the kernel execution.
   */
  proof: Proof;
}

/**
 * ProofCreator provides functionality to create and validate proofs, and retrieve
 * siloed noteHashes necessary for maintaining transaction privacy and security on the network.
 */
export interface ProofCreator {
  /**
   * Computes the siloed noteHashes for a given set of public inputs.
   *
   * @param publicInputs - The public inputs containing the contract address and new noteHashes to be used in generating siloed noteHashes.
   * @returns An array of Fr (finite field) elements representing the siloed noteHashes.
   */
  getSiloedNoteHashes(publicInputs: PrivateCircuitPublicInputs): Promise<Fr[]>;

  /**
   * Creates a proof output for a given signed transaction request and private call data for the first iteration.
   *
   * @param privateKernelInputsInit - The private data structure for the initial iteration.
   * @returns A Promise resolving to a ProofOutput object containing public inputs and the kernel proof.
   */
  createProofInit(privateKernelInputsInit: PrivateKernelInputsInit): Promise<ProofOutput>;

  /**
   * Creates a proof output for a given previous kernel data and private call data for an inner iteration.
   *
   * @param privateKernelInputsInner - The private input data structure for the inner iteration.
   * @returns A Promise resolving to a ProofOutput object containing public inputs and the kernel proof.
   */
  createProofInner(privateKernelInputsInner: PrivateKernelInputsInner): Promise<ProofOutput>;

  /**
   * Creates a proof output based on the last inner kernel iteration kernel data for the final ordering iteration.
   *
   * @param privateKernelInputsOrdering - The private input data structure for the final ordering iteration.
   * @returns A Promise resolving to a ProofOutput object containing public inputs and the kernel proof.
   */
  createProofOrdering(privateKernelInputsOrdering: PrivateKernelInputsOrdering): Promise<ProofOutputFinal>;
}

/**
 * The KernelProofCreator class is responsible for generating siloed noteHashes and zero-knowledge proofs
 * for private kernel circuit. It leverages Barretenberg and Circuits Wasm libraries
 * to perform cryptographic operations and proof creation. The class provides methods to compute noteHashes
 * based on the given public inputs and to generate proofs based on signed transaction requests, previous kernel
 * data, private call data, and a flag indicating whether it's the first iteration or not.
 */
export class KernelProofCreator implements ProofCreator {
  constructor(private log = createDebugLogger('aztec:kernel_proof_creator')) {}

  public async getSiloedNoteHashes(publicInputs: PrivateCircuitPublicInputs) {
    const wasm = await CircuitsWasm.get();
    const contractAddress = publicInputs.callContext.storageContractAddress;

    return publicInputs.newNoteHashes.map(noteHash => siloNoteHash(wasm, contractAddress, noteHash));
  }

  public async createProofInit(privateInputs: PrivateKernelInputsInit): Promise<ProofOutput> {
    const wasm = await CircuitsWasm.get();
    this.log('Executing private kernel simulation init...');
    const result = privateKernelSimInit(wasm, privateInputs);
    if (result instanceof CircuitError) {
      throw new CircuitError(result.code, result.message);
    }
    this.log('Skipping private kernel init proving...');
    // TODO
    const proof = makeEmptyProof();
    this.log('Kernel Prover Init Completed!');

    return {
      publicInputs: result,
      proof: proof,
    };
  }

  public async createProofInner(privateInputs: PrivateKernelInputsInner): Promise<ProofOutput> {
    const wasm = await CircuitsWasm.get();
    this.log('Executing private kernel simulation inner...');
    const result = privateKernelSimInner(wasm, privateInputs);
    if (result instanceof CircuitError) {
      throw new CircuitError(result.code, result.message);
    }
    this.log('Skipping private kernel inner proving...');
    // TODO
    const proof = makeEmptyProof();
    this.log('Kernel Prover Inner Completed!');

    return {
      publicInputs: result,
      proof: proof,
    };
  }

  public async createProofOrdering(privateInputs: PrivateKernelInputsOrdering): Promise<ProofOutputFinal> {
    const wasm = await CircuitsWasm.get();
    this.log('Executing private kernel simulation ordering...');
    const result = privateKernelSimOrdering(wasm, privateInputs);
    if (result instanceof CircuitError) {
      throw new CircuitError(result.code, result.message);
    }
    this.log('Skipping private kernel ordering proving...');
    // TODO
    const proof = makeEmptyProof();
    this.log('Ordering Kernel Prover Ordering Completed!');

    const publicInputs = result as KernelCircuitPublicInputsFinal;

    return {
      publicInputs,
      proof,
    };
  }
}
