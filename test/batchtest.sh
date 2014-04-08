#!/bin/bash

[ "$#" -lt 2 ] && echo "The Usage of Test:  \
		testcli 0 $用户帐号				用户基本信息查询 
		testcli 1 $用户帐号 $银行卡					银行扣款协议确认\n            
		testcli 2 $用户帐号 $银行卡					行扣款协议修改 
		testcli 3 $用户帐号 $银行卡					银行扣款协议删除\n     
		testcli 4 $用户帐号 						缴费信息查询
		testcli 5 $用户帐号 						缴费确认\n                
		testcli 6 冲正  7 单笔缴费对帐\n    "      
i=0;
hh=$2
user=${hh:-11111111111}
nzh=$3
bank=${nzh:-22222222222222222222}
echo "./testcli $1 $user $bank"
./testcli $1 $user $bank
#while [ "$i" != "10" ]
#do
#	./testcli $i $u $b
#	i=$(($i+1))
#done
