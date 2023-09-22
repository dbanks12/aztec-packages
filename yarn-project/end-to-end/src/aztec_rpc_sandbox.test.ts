import { aztecRpcTestSuite } from '@aztec/aztec-rpc';
import { createAztecRpcClient, waitForSandbox } from '@aztec/aztec.js';

const { SANDBOX_URL = 'http://localhost:8080' } = process.env;

const setup = async () => {
  const aztecRpc = createAztecRpcClient(SANDBOX_URL);
  await waitForSandbox(aztecRpc);
  return aztecRpc;
};

aztecRpcTestSuite('aztec_rpc_sandbox', setup);
