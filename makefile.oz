makefile(
   lib : [
	  'Example.exe'
	  'json.so'
	 ]
   rules:o('json.so':ld('json.o' [library('jansson')]))
   uri: 'x-ozlib://doublec/json'
   mogul: 'mogul:/doublec/json'
   blurb: "Encode and Decode JSON using libjansson"
)
