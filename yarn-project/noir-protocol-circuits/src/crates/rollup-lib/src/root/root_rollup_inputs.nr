use crate::abis::nullifier_leaf_preimage::NullifierLeafPreimage;
use crate::abis::previous_rollup_data::PreviousRollupData;

use crate::abis::append_only_tree_snapshot::AppendOnlyTreeSnapshot;
use crate::abis::constant_rollup_data::ConstantRollupData;
use dep::aztec::constants_gen::{
    NUMBER_OF_L1_L2_MESSAGES_PER_ROLLUP,
    L1_TO_L2_MSG_SUBTREE_SIBLING_PATH_LENGTH,
    HISTORIC_BLOCKS_TREE_HEIGHT
};

struct RootRollupInputs {
    // All below are shared between the base and merge rollups
    previous_rollup_data : [PreviousRollupData; 2],

    // inputs required to process l1 to l2 messages
    new_l1_to_l2_messages : [Field; NUMBER_OF_L1_L2_MESSAGES_PER_ROLLUP],
    new_l1_to_l2_messages_tree_root_sibling_path : [Field; L1_TO_L2_MSG_SUBTREE_SIBLING_PATH_LENGTH],

    start_l1_to_l2_messages_tree_snapshot : AppendOnlyTreeSnapshot,

    // inputs required to add the block hash
    start_historic_blocks_tree_snapshot : AppendOnlyTreeSnapshot,
    new_historic_blocks_tree_sibling_path : [Field; HISTORIC_BLOCKS_TREE_HEIGHT],
}