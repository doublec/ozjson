# JSON encoding/decoding for Mozart/Oz

This library contains functions encoding Oz records into JSON and vice versa:

    ozmake

Installation:

    ozmake --install

Import of the library can be done in the OPI with:

    declare [JSON] = {Module.link ['x-ozlib://doublec/json/json.so{native}']}

Currently the library exposes the following functions:

* `{JSON.encode ozrecord}` - Convert an Oz record into a JSON string: 

        {Browse {JSON.encode json(one:1 two:"hello")}}

* `{JSON.decode string}` - Convert a string of JSON code into an Oz record: 

        {Browse {JSON.decode "{\"one\":1,\"two\":\"hello\"}"}}

You'll need the following C library to build:

* [libjansson](http://www.digip.org/jansson/)

Recent versions of this library can be obtained from:

    http://github.com/doublec/ozjson

The author can be reached at:

    Chris Double
    chris.double@double.co.nz
    http://www.bluishcoder.co.nz
