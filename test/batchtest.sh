#!/bin/bash

[ "$#" -lt 2 ] && echo "The Usage of Test:  \
		testcli 0 $�û��ʺ�				�û�������Ϣ��ѯ 
		testcli 1 $�û��ʺ� $���п�					���пۿ�Э��ȷ��\n            
		testcli 2 $�û��ʺ� $���п�					�пۿ�Э���޸� 
		testcli 3 $�û��ʺ� $���п�					���пۿ�Э��ɾ��\n     
		testcli 4 $�û��ʺ� 						�ɷ���Ϣ��ѯ
		testcli 5 $�û��ʺ� 						�ɷ�ȷ��\n                
		testcli 6 ����  7 ���ʽɷѶ���\n    "      
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
