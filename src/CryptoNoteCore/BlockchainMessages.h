/*
 * Copyright (c) 2017-2018, The Alloy Developers.
 * Portions Copyright (c) 2012-2017, The CryptoNote Developers, The Bytecoin Developers.
 *
 * This file is part of Alloy.
 *
 * This file is subject to the terms and conditions defined in the
 * file 'LICENSE', which is part of this source code package.
 */

#pragma once

#include <functional>
#include <vector>

#include <CryptoNote.h>

namespace CryptoNote {

namespace Messages {
// immutable messages
struct NewBlock {
  uint32_t blockIndex;
  Crypto::Hash blockHash;
};

struct NewAlternativeBlock {
  uint32_t blockIndex;
  Crypto::Hash blockHash;
};

struct ChainSwitch {
  uint32_t commonRootIndex;
  std::vector<Crypto::Hash> blocksFromCommonRoot;
};

struct AddTransaction {
  std::vector<Crypto::Hash> hashes;
};

struct DeleteTransaction {
  std::vector<Crypto::Hash> hashes;
  enum class Reason {
    InBlock,
    Outdated,
    NotActual
  } reason;
};
}

class BlockchainMessage {
public:
  enum class Type {
    NewBlock,
    NewAlternativeBlock,
    ChainSwitch,
    AddTransaction,
    DeleteTransaction
  };

  using NewBlock = Messages::NewBlock;
  using NewAlternativeBlock = Messages::NewAlternativeBlock;
  using ChainSwitch = Messages::ChainSwitch;
  using AddTransaction = Messages::AddTransaction;
  using DeleteTransaction = Messages::DeleteTransaction;

  BlockchainMessage(const NewBlock& message);
  BlockchainMessage(const NewAlternativeBlock& message);
  BlockchainMessage(const ChainSwitch& message);
  BlockchainMessage(const AddTransaction& message);
  BlockchainMessage(const DeleteTransaction& message);

  BlockchainMessage(const BlockchainMessage& other);

  ~BlockchainMessage();

  // pattern matchin API
  void match(std::function<void(const NewBlock&)>, std::function<void(const NewAlternativeBlock&)>,
             std::function<void(const ChainSwitch&)>, std::function<void(const AddTransaction&)>,
             std::function<void(const DeleteTransaction&)>) const;

  // API with explicit type handling
  Type getType() const;
  const NewBlock& getNewBlock() const;
  const NewAlternativeBlock& getNewAlternativeBlock() const;
  const ChainSwitch& getChainSwitch() const;
  const AddTransaction& getAddTransaction() const;
  const DeleteTransaction& getDeleteTransaction() const;

private:
  const Type type;
  union {
    NewBlock newBlock;
    NewAlternativeBlock newAlternativeBlock;
    ChainSwitch* chainSwitch;
    AddTransaction* addTransaction;
    DeleteTransaction* deleteTransaction;
  };
};

// factory functions
BlockchainMessage makeChainSwitchMessage(uint32_t index, std::vector<Crypto::Hash>&& hashes);
BlockchainMessage makeNewAlternativeBlockMessage(uint32_t index, const Crypto::Hash& hash);
BlockchainMessage makeNewBlockMessage(uint32_t index, const Crypto::Hash& hash);
BlockchainMessage makeAddTransactionMessage(std::vector<Crypto::Hash>&& hash);
BlockchainMessage makeDelTransactionMessage(std::vector<Crypto::Hash>&& hash, Messages::DeleteTransaction::Reason r);
}
