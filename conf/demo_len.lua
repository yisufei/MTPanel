
local config = {
    common = {
        req = {
            content = {
                { name = 'TxnCod',      type = 'string', length = 6, trim = true },
                { name = 'SubSystem',   type = 'string', length = 8, trim = true },
                { name = 'TermDate',    type = 'string', length = 8, trim = true },
                { name = 'TermTime',    type = 'string', length = 6, trim = true },
                { name = 'TermSeq',     type = 'string', length = 32, trim = true },
                { name = 'Branch',      type = 'string', length = 9, trim = true },
                { name = 'Teller',      type = 'string', length = 10, trim = true },
                { name = 'Channel',     type = 'string', length = 2, trim = true }
            }
        },
        ans = {
            content = {
                { name = 'TxnCod',      type = 'string', length = 6, trim = true },
                { name = 'TranDate',    type = 'string', length = 8, trim = true },
                { name = 'TranTime',    type = 'string', length = 6, trim = true },
                { name = 'TranSeq',     type = 'string', length = 20, trim = true },
                { name = 'RspCod',      type = 'string', length = 4, trim = true },
                { name = 'RspMsg',      type = 'string', length = 256, trim = true }
            }
        }
    },

    QRY00001 = {
        req = {
                { name = 'BDRAC22',     type = 'string', length = 22, trim = true }
        },
        ans = {
                { name = 'BDOAC221',    type = 'string', length = 22, trim = true },
                { name = 'CARDNO',      type = 'string', length = 22, trim = true },
                { name = 'BDOCARDFLAG', type = 'string', length = 1, trim = true },
                { name = 'CARD_LEVEL',  type = 'string', length = 1, trim = true },
                { name = 'BDOCMNO1',    type = 'string', length = 23, trim = true },
                { name = 'CUST_NAME',   type = 'string', length = 80, trim = true },
                { name = 'CUST_TYPE',   type = 'string', length = 1, trim = true },
                { name = 'ACCT_TYPE',   type = 'string', length = 1, trim = true },
                { name = 'BDOFLNM1',    type = 'string', length = 80, trim = true },
                { name = 'BDOAMT1',     type = 'string', length = 17, trim = true },
                { name = 'BDOAMT2',     type = 'string', length = 17, trim = true },
                { name = 'BDOCFTP1',    type = 'string', length = 3, trim = true },
                { name = 'BDOCFNO1',    type = 'string', length = 20, trim = true },
                { name = 'BDOACST1',    type = 'string', length = 1, trim = true },
                { name = 'CardStat',    type = 'string', length = 20, trim = true },
                { name = 'BDOBRNO1',    type = 'string', length = 6, trim = true },
                { name = 'BDODATE1',    type = 'string', length = 8, trim = true },
                { name = 'BDODRWT1',    type = 'string', length = 10, trim = true },
                { name = 'bankno',      type = 'string', length = 6, trim = true },
                { name = 'instreprname',type = 'string', length = 80, trim = true },
                { name = 'instrepridtype',type = 'string', length = 3, trim = true },
                { name = 'instrepridcode',type = 'string', length = 20, trim = true },
                { name = 'address',     type = 'string', length = 80, trim = true },
                { name = 'postcode',    type = 'string', length = 10, trim = true },
                { name = 'remark',      type = 'string', length = 80, trim = true },
                { name = 'subsys',      type = 'string', length = 3, trim = true }
        }
    }
}

return config
