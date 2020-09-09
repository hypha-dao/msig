
# Step 1: Create Transaction file
### cleos
```
cleos -u https://test.telos.kitchen set contract -sjd -x 186400 test.hypha test/test > test_deploy.json
```

### eosc 
```
eosc -u https://test.telos.kitchen --vault-file ../mt.hypha.json system setcontract test.hypha test/test.wasm test/test.abi --write-transaction test_deploy.json
```

# Step 2: Modify transaction file
> NOTE: TODO: we can automate this step with ```daoctl``` soon.

You must modify the transaction file to include additional parameters:

Before (head):
``` json
{
  "expiration": "2020-09-09T14:57:48",
  "ref_block_num": 11290,
  "ref_block_prefix": 155158750,
  "max_net_usage_words": 0,
  "max_cpu_usage_ms": 0,
  "delay_sec": 0,
  "context_free_actions": [],
  "actions": [
    {
      "account": "eosio",
      "name": "setcode",
```

After (head):
> TODO: add document hash parameter to action, and document can contain link to Github, notes, etc...
``` json
{
  "proposer": "proposal_name",
  "proposal_name": "mytestdeploy",
  "requested": [
    {
      "actor": "buckyjohnson",
      "permission": "active"
    },
    {
      "actor": "mt.hypha",
      "permission": "active"
    }
  ],
  "trx": {
    "expiration": "2020-09-09T14:57:48",
    "ref_block_num": 11290,
    "ref_block_prefix": 155158750,
    "max_net_usage_words": 0,
    "max_cpu_usage_ms": 0,
    "delay_sec": 0,
    "context_free_actions": [],
    "actions": [
      {
        "account": "eosio",
        "name": "setcode",
```

# Step 3: Propose transaction
### cleos 
``` sh
cleos -u http://test.telos.kitchen push action msig.hypha propose ./test_deploy.json -p mt.hypha
```

### eosc
> NOTE: need to test this
``` sh
eosc -u https://test.telos.kitchen --vault-file ../mt.hypha.json tx create msig.hypha propose '{
  "proposer": "mt.hypha",
  "proposal_name": "mytestdeploy",
  "requested": [
    {
      "actor": "buckyjohnson",
      "permission": "active"
    },
    {
      "actor": "mt.hypha",
      "permission": "active"
    }
  ],
  "trx": {
    "expiration": "2020-09-10T14:57:48",
    "ref_block_num": 11290,
    "ref_block_prefix": 155158750,
    "max_net_usage_words": 0,
    "max_cpu_usage_ms": 0,
    "delay_sec": 0,
    "context_free_actions": [],
    "actions": [
      {
        "account": "eosio",
        "name": "setcode",
        "authorization": [
          {
            "actor": "test.hypha",
            "permission": "active"
          }
        ],
        "data": "008069d53790b1ca0000f8060061736d0100000001370b60027f7f0060017e0060017f006000017f60037f7f7f017f60027f7f017f60027f7e0060000060037e7e7e0060017f017f60027e7e00029d010903656e76087072696e74735f6c000003656e76067072696e746e000103656e76067072696e7473000203656e7610616374696f6e5f646174615f73697a65000303656e760c656f73696f5f617373657274000003656e76066d656d736574000403656e7610726561645f616374696f6e5f64617461000503656e76066d656d637079000403656e7611656f73696f5f6173736572745f636f6465000603070607080907020a0405017001010105030100010616037f014180c0000b7f0041b8c0000b7f0041b8c0000b070901056170706c79000a0a9a04060400100c0b7a001009200020015104404280808080808080c0eb00200251044020002001100e052000428080808080c0ba98d500520440410042808080d9d3b3ed82ef0010080b0b05428080808080c0ba98d50020015104404280808080aefadeeaa47f2002510440410042818080d9d3b3ed82ef0010080b0b0b4100100d0b800101037f02400240024002402000450d004100410028028c40200041107622016a220236028c404100410028028440220320006a41076a417871220036028440200241107420004d0d0120014000417f460d020c030b41000f0b4100200241016a36028c40200141016a4000417f470d010b41004190c000100420030f0b20030b3601017f230041106b2200410036020c4100200028020c28020041076a417871220036028440410020003602804041003f0036028c400b02000bdb0102047f017e230041106b22022103200224000240024002400240024010032204450d002004418004490d012004100b21020c020b2003420037030841002102200341086a21050c020b20022004410f6a4170716b220224000b2002200410061a20034200370308200341086a2105200441074b0d010b410041b3c00010040b20052002410810071a41a9c000210420032903082106024041a9c0002d00002202450d000240034020024125460d01200441011000200441016a22042d00002202450d020c000b0b20061001200441016a10020b200341106a24000b0b4003004190c0000b236661696c656420746f20616c6c6f63617465207061676573004e616d65203a20250a000041b3c0000b0572656164000041000b0438200000"
      },
      {
        "account": "eosio",
        "name": "setabi",
        "authorization": [
          {
            "actor": "test.hypha",
            "permission": "active"
          }
        ],
        "data": "008069d53790b1ca570e656f73696f3a3a6162692f312e3100010268690001026e6d046e616d6501000000000000806b026869275374756220666f7220686920616374696f6e27732072696361726469616e20636f6e74726163740000000000"
      }
    ],
    "transaction_extensions": [],
    "signatures": [
      "SIG_K1_JzDyLa9x6CkZ3ePT8Q4qf1u93Z8xF4XudJt8mtuCkEeiwCTvEKd6xncm7KChzLMTGim9NiTNmU19Hpgzj5RRSkx6i96chh"
    ],
    "context_free_data": [],
    "chain_id": "1eaa0824707c8c16bd25145493bf062aecddfeb56c736f6ba6397f3195f33c9f"
  }
}' -p mt.hypha
```

Transaction will look like [this](https://eosauthority.com/transaction/7b5cd37768bd3d0625883064f03d195a06d5bf23d32f50f95947df813ae67729?network=telostest)

# Step 4: Approve proposal
> TODO: we can build a small UI screen in the DHO for linking to Github and running approvals
### cleos 
``` sh
# approve for buckyjohnson
cleos -u https://test.telos.kitchen push action msig.hypha approve '{
  "proposer":"mt.hypha",
  "proposal_name":"mytestdeploy",
  "level":
    {
      "actor": "buckyjohnson",
      "permission": "active"
    }
}' -p buckyjohnson
```

### eosc
``` sh
# approve for buckyjohnson
eosc -u https://test.telos.kitchen --vault-file ../../buckyjohnson/tk-dev.json tx create msig.hypha approve '{
  "proposer":"mt.hypha",
  "proposal_name":"test2",
  "level": 
    {
      "actor": "buckyjohnson", 
      "permission": "active"
    }
}' -p buckyjohnson
```

# Step 4: Check approvals
You can query if specific users have approved this proposal by checking the approvals table:
> TODO: we can change this data structure to avoid scopes
> NOTE: prior to approving, the permission level and time is in the ```requested_approvals``` vector. The ```approve``` action moves it to ```provided_approvals```.
``` json
$ eosc -u https://test.telos.kitchen get table msig.hypha mt.hypha approvals2
{
  "more": false,
  "rows": [
    {
      "version": 1,
      "proposal_name": "mytestdeploy",
      "requested_approvals": [],
      "provided_approvals": [
        {
          "level": {
            "actor": "mt.hypha",
            "permission": "active"
          },
          "time": "2020-09-09T16:18:32.000"
        },
        {
          "level": {
            "actor": "buckyjohnson",
            "permission": "active"
          },
          "time": "2020-09-09T16:37:53.000"
        }
      ]
    }
  ]
}
```

# Step 5: After enough approvals, execute proposal
### cleos 
``` sh
cleos -u http://test.telos.kitchen push action msig.hypha exec '["mt.hypha","test2","mt.hypha"] -p mt.hypha
```

### eosc
> NOTE: need to test this
``` sh
eosc -u https://test.telos.kitchen --vault-file ../mt.hypha.json tx create msig.hypha exec '{
  "proposer":"mt.hypha",
  "proposal_name":"test2",
  "executer": "mt.hypha"
}' -p mt.hypha
```


# Setting permissions

The system account ```eosio.msig``` is delegated to ```eosio@active```, the sudo permission.  In order to allow ```msig.hypha``` to update ```test.hypha```, the account we are 

``` json
eosc -u https://test.telos.kitchen --vault-file ../eosc-testnet-vault.json tx create eosio updateauth '{
    "account": "test.hypha",
    "permission": "active",
    "parent": "owner",
    "auth": {
        "keys":[],
        "threshold": 2,
        "accounts": [
          {
            "permission": {
                    "actor": "buckyjohnson",
                    "permission": "active"
                },
                "weight": 1
          },
            {
                "permission": {
                    "actor": "msig.hypha",
                    "permission": "eosio.code"
                },
                "weight": 2
            },
             {
            "permission": {
                    "actor": "mt.hypha",
                    "permission": "active"
                },
                "weight": 1
          },
            {
                "permission": {
                    "actor": "test.hypha",
                    "permission": "eosio.code"
                },
                "weight": 2
            }
        ],
        "waits": []
    }
}' -p test.hypha@owner
```