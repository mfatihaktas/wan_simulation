server_sf=server.c
server_tf=server
client_sf=client.c
client_tf=client

server: $(server_sf)	
		gcc -o $(server_tf) $<

client: $(client_sf)
		gcc -o $(client_tf) $<

clean:
		rm -rf $(server_tf) $(client_tf)
