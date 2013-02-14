server_sf=server.c
server_tf=server
client_sf=client.c
client_tf=client
producer_sf=producer.c
producer_tf=producer
gateway_sf=gateway.c
gateway_tf=gateway
dsanode_sf=dsanode.c
dsanode_tf=dsanode

server: $(server_sf)	
		gcc -o $(server_tf) $<

client: $(client_sf)
		gcc -o $(client_tf) $<
producer: $(producer_sf)
		gcc -o $(producer_tf) $<
gateway: $(gateway_sf)
		gcc -pthread -o $(gateway_tf) $<
dsanode: $(dsanode_sf)
		gcc -o $(dsanode_tf) $<
		
clean:
		rm -rf $(server_tf) $(client_tf) $(producer_tf) $(gateway_tf)
