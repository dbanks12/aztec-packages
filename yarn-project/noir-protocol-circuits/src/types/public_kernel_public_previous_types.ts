/* Autogenerated file, do not edit! */

/* eslint-disable */

export type FixedLengthArray<T, L extends number> = L extends 0 ? never[] : T[] & { length: L };

export type Field = string;
export type u32 = string;

export interface AggregationObject {}

export interface Address {
  inner: Field;
}

export interface CallerContext {
  msg_sender: Address;
  storage_contract_address: Address;
}

export interface CallRequest {
  hash: Field;
  caller_contract_address: Address;
  caller_context: CallerContext;
}

export interface EthAddress {
  inner: Field;
}

export interface NewContractData {
  contract_address: Address;
  portal_contract_address: EthAddress;
  function_tree_root: Field;
}

export interface FunctionSelector {
  inner: u32;
}

export interface FunctionData {
  selector: FunctionSelector;
  is_internal: boolean;
  is_private: boolean;
  is_constructor: boolean;
}

export interface OptionallyRevealedData {
  call_stack_item_hash: Field;
  function_data: FunctionData;
  vk_hash: Field;
  portal_contract_address: EthAddress;
  pay_fee_from_l1: boolean;
  pay_fee_from_public_l2: boolean;
  called_from_l1: boolean;
  called_from_public_l2: boolean;
}

export interface PublicDataUpdateRequest {
  leaf_index: Field;
  old_value: Field;
  new_value: Field;
}

export interface PublicDataRead {
  leaf_index: Field;
  value: Field;
}

export interface CombinedAccumulatedData {
  aggregation_object: AggregationObject;
  read_requests: FixedLengthArray<Field, 128>;
  pending_read_requests: FixedLengthArray<Field, 128>;
  new_commitments: FixedLengthArray<Field, 64>;
  new_nullifiers: FixedLengthArray<Field, 64>;
  nullified_commitments: FixedLengthArray<Field, 64>;
  private_call_stack: FixedLengthArray<CallRequest, 8>;
  public_call_stack: FixedLengthArray<CallRequest, 8>;
  new_l2_to_l1_msgs: FixedLengthArray<Field, 2>;
  encrypted_logs_hash: FixedLengthArray<Field, 2>;
  unencrypted_logs_hash: FixedLengthArray<Field, 2>;
  encrypted_log_preimages_length: Field;
  unencrypted_log_preimages_length: Field;
  new_contracts: FixedLengthArray<NewContractData, 1>;
  optionally_revealed_data: FixedLengthArray<OptionallyRevealedData, 4>;
  public_data_update_requests: FixedLengthArray<PublicDataUpdateRequest, 16>;
  public_data_reads: FixedLengthArray<PublicDataRead, 16>;
}

export interface Block {
  note_hash_tree_root: Field;
  nullifier_tree_root: Field;
  contract_tree_root: Field;
  l1_to_l2_data_tree_root: Field;
  public_data_tree_root: Field;
  global_variables_hash: Field;
}

export interface HistoricalBlockData {
  blocks_tree_root: Field;
  block: Block;
  private_kernel_vk_tree_root: Field;
}

export interface Point {
  x: Field;
  y: Field;
}

export interface ContractDeploymentData {
  deployer_public_key: Point;
  constructor_vk_hash: Field;
  function_tree_root: Field;
  contract_address_salt: Field;
  portal_contract_address: EthAddress;
}

export interface TxContext {
  is_fee_payment_tx: boolean;
  is_rebate_payment_tx: boolean;
  is_contract_deployment_tx: boolean;
  contract_deployment_data: ContractDeploymentData;
  chain_id: Field;
  version: Field;
}

export interface CombinedConstantData {
  block_data: HistoricalBlockData;
  tx_context: TxContext;
}

export interface KernelCircuitPublicInputs {
  end: CombinedAccumulatedData;
  constants: CombinedConstantData;
  is_private: boolean;
}

export interface Proof {}

export interface VerificationKey {}

export interface PreviousKernelData {
  public_inputs: KernelCircuitPublicInputs;
  proof: Proof;
  vk: VerificationKey;
  vk_index: u32;
  vk_path: FixedLengthArray<Field, 3>;
}

export interface CallContext {
  msg_sender: Address;
  storage_contract_address: Address;
  portal_contract_address: EthAddress;
  function_selector: FunctionSelector;
  is_delegate_call: boolean;
  is_static_call: boolean;
  is_contract_deployment: boolean;
}

export interface StorageUpdateRequest {
  storage_slot: Field;
  old_value: Field;
  new_value: Field;
}

export interface StorageRead {
  storage_slot: Field;
  current_value: Field;
}

export interface PublicCircuitPublicInputs {
  call_context: CallContext;
  args_hash: Field;
  return_values: FixedLengthArray<Field, 4>;
  contract_storage_update_requests: FixedLengthArray<StorageUpdateRequest, 16>;
  contract_storage_reads: FixedLengthArray<StorageRead, 16>;
  public_call_stack_hashes: FixedLengthArray<Field, 4>;
  new_commitments: FixedLengthArray<Field, 16>;
  new_nullifiers: FixedLengthArray<Field, 16>;
  new_l2_to_l1_msgs: FixedLengthArray<Field, 2>;
  unencrypted_logs_hash: FixedLengthArray<Field, 2>;
  unencrypted_log_preimages_length: Field;
  historical_block_data: HistoricalBlockData;
  prover_address: Address;
}

export interface PublicCallStackItem {
  contract_address: Address;
  public_inputs: PublicCircuitPublicInputs;
  function_data: FunctionData;
  is_execution_request: boolean;
}

export interface PublicCallData {
  call_stack_item: PublicCallStackItem;
  public_call_stack: FixedLengthArray<CallRequest, 4>;
  proof: Proof;
  portal_contract_address: EthAddress;
  bytecode_hash: Field;
}

export interface PublicKernelPublicPreviousInputs {
  previous_kernel: PreviousKernelData;
  public_call: PublicCallData;
}

export type ReturnType = KernelCircuitPublicInputs;

export interface InputType {
  input: PublicKernelPublicPreviousInputs;
}
