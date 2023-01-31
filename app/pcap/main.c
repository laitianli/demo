#include <pcap.h>
#include <stdio.h>
#include <unistd.h>

 int main(int argc, char *argv[])
 {
	pcap_t *handle;            /* Session handle */
	char *dev;            /* The device to sniff on */
	char errbuf[PCAP_ERRBUF_SIZE];    /* Error string */
	struct bpf_program fp;        /* The compiled filter */
	char filter_exp[] = "port 80";    /* The filter expression */
	bpf_u_int32 mask = 0;        /* Our netmask */
	bpf_u_int32 net = 0;        /* Our IP */
	struct pcap_pkthdr header;    /* The header that pcap gives us */
	const u_char *packet;        /* The actual packet */
#if 0
	/* Define the device */
	dev = pcap_lookupdev(errbuf);
	if (dev == NULL) {
		fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
		return(2);
	}
#endif
	dev = "ens33";
	/* Find the properties for the device */
	if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
		fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
		net = 0;
		mask = 0;
	}
	printf("net = %d,",net);
	printf("mask = %d,",mask);
	/* Open the session in promiscuous mode */
	handle = pcap_open_live(dev, BUFSIZ, 1, 10000, errbuf);
	if (handle == NULL) {
		fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
		return(2);
	}
	pcap_set_snaplen(handle, 200);
	/* Compile and apply the filter */
	if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
		fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
		return(2);
	}
	if (pcap_setfilter(handle, &fp) == -1) {
		fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
		return(2);
	}
	/* Grab a packet */
	packet = pcap_next(handle, &header);
	/* Print its length */
	printf("Jacked a packet with length of [%d]\n", header.caplen);
	/* And close the session */
	pcap_close(handle);
	return(0);
}