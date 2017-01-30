

server = {
	demo = {
	    ip = '0.0.0.0',
	    port = 7000,
	    request = {
	        headLen = 8,
	    },
	    mode = 'debug'
	},

	demo2 = {
		ip = '0.0.0.0',
		port = 7001,
		request = {
			headLen = 6,
		},
		mode = 'release'
	},
}

--print(server[ip])
