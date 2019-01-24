# The Operator Foundation

[Operator](https://operatorfoundation.org) makes useable tools to help people around the world with censorship, security, and privacy.

## U2F Authentication for OpenVPN

This project is part of a suite of a suite of tools for adding U2F authentication to OpenVPN.

### u2f-auth-server-cli

This commmand line tool is meant to be used in conjunction with the U2F auth server plugin for OpenVPN. This tool allows
server administrators to manage the configuration file used by the plugin.

#### Installation

Building the command line tool requires CMake to be installed.

To build using CMake:

    cmake .
    make

This will build the u2f-server-auth-cli executable.

#### Usage

Add a user

    ./u2f-server-auth-cli -a username

Set the user's password
    
    ./u2f-server-auth-cli -p password username

Enable U2F for the user
    
    ./u2f-server-auth-cli -e username
    
Disable U2F for the user
    
    ./u2f-server-auth-cli -d username
    
Enable self-registration for the user

    ./u2f-server-auth-cli -s username
    
Disable self-registration for the user

    ./u2f-server-auth-cli -n username
    
Register a key for the user. The key is generated using the u2f-server command line tool.
    
    ./u2f-server-auth-cli -g userkeyfilename -k keyhandlefilename username

Unregister the user's key:    
    
    ./u2f-server-auth-cli -u username

Make U2F required for the user:
    
    ./u2f-server-auth-cli -q username
    
Make U2F optional for the user:    
    
    ./u2f-server-auth-cli -o username

Delete a user

    ./u2f-server-auth-cli -r username

#### Registering Keys

Registering keys requires using both the u2f-server and u2f-host tools, which are provided by Yubikey.

#### Generate a challenge

The first step is to generate a U2F challenge:

    u2f-server -aregister -ohttp://demo.yubico.com -i http://demo.yubico.com -k keyhandle.dat -p userkey.dat
    
The output will be a JSON string, such as this:

    { "challenge": "cOQ-TBjhsbiAss7_hg2LhNNWxqjrRPWoSlThnQD6e2I",\ "version":"U2F_V2", "appId": "http:\/\/demo.yubico.com" }
    
The u2f-server will now await the response.
    
#### Generate a response

The next step is to generate a U2F response on the client:

    u2f-host -aregister -o http://demo.yubico.com
    
This step must be done on the client that has the U2F device. Copy and paste the challenge from the previous step into
the input of u2f-host. Press Ctrl-D to signal the end of input. On some operating systems, the challenge will be too
large to be pasted at once. In this case, you will need to paste as much as possible, hit Ctrl-D once, paste the rest,
and then press Ctrl-D twice to signal the end of input.

Once the JSON has been pasted into the input of u2f-host, the U2F device will require user interaction. For instance, a
Yubikey will flash and await a touch by the user. Once user interaction with the U2F device has completed, u2f-host will
print a response, such as this:

    { "registrationData": "BQQcsmA1brUvrNgntjvKrCzUIIN92Y61ee\
    DI7xnEVliksvO8l0aVY0HcASqf5dExipOVHTfxYVImZ_M3U4eAjt-OQCt\
    h1BOT0gr3HLkLJcid1Ahks8NyjeyfkoLLES-i4Fn650vkgE7jTOnCGiuC\
    nZzpkZ2exkwLWhNW2QZA1nAaS4owggIbMIIBBaADAgECAgR1o_Z1MAsGC\
    SqGSIb3DQEBCzAuMSwwKgYDVQQDEyNZdWJpY28gVTJGIFJvb3QgQ0EgU2\
    VyaWFsIDQ1NzIwMDYzMTAgFw0xNDA4MDEwMDAwMDBaGA8yMDUwMDkwNDA\
    wMDAwMFowKjEoMCYGA1UEAwwfWXViaWNvIFUyRiBFRSBTZXJpYWwgMTk3\
    MzY3OTczMzBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABBmjfkNqa2mXz\
    Vh2ZxuES5coCvvENxDMDLmfd-0ACG0Fu7wR4ZTjKd9KAuidySpfona5cs\
    GmlM0Te_Zu35h_wwujEjAQMA4GCisGAQQBgsQKAQIEADALBgkqhkiG9w0\
    BAQsDggEBAb0tuI0-CzSxBg4cAlyD6UyT4cKyJZGVhWdtPgj_mWepT3Tu\
    9jXtdgA5F3jfZtTc2eGxuS-PPvqRAkZd40AXgM8A0YaXPwlT4s0RUTY9Y\
    8aAQzQZeAHuZk3lKKd_LUCg5077dzdt90lC5eVTEduj6cOnHEqnOr2Cv7\
    5FuiQXX7QkGQxtoD-otgvhZ2Fjk29o7Iy9ik7ewHGXOfoVw_ruGWi0YfX\
    BTuqEJ6H666vvMN4BZWHtzhC0k5ceQslB9Xdntky-GQgDqNkkBf32GKwA\
    FT9JJrkO2BfsB-wfBrTiHr0AABYNTNKTceA5dtR3UVpI492VUWQbY3YmW\
    UUfKTI7fM4wRQIhAJNrBRxUWPwdVWFpuutWd78ESaoZFXvK2yvUzm14cP\
    vQAiBFSn-5J6LvJAstgSFLD-1vWXANqrJ7-5yoIqT6fOj9JQ==", "cli\
    entData": "eyAiY2hhbGxlbmdlIjogImNPUS1UQmpoc2JpQXNzN19oZz\
    JMaE5OV3hxanJSUFdvU2xUaG5RRDZlMkkiLCAib3JpZ2luIjogImh0dHA\
    6XC9cL2RlbW8ueXViaWNvLmNvbSIsICJ0eXAiOiAibmF2aWdhdG9yLmlk\
    LmZpbmlzaEVucm9sbG1lbnQiIH0=" }
    
#### Paste the response into u2f-server

Similar to the process for putting the challenge into the input of u2f-host, the response should be copied and pasted
into the input of the waiting u2f-server. Press Ctrl-D to signal the end of input. On some operating systems, the
challenge will be too large to be pasted at once. In this case, you will need to paste as much as possible, hit Ctrl-D
once, paste the rest, and then press Ctrl-D twice to signal the end of input.

#### Register the key with u2f-server-auth-cli

The u2f-server command from the first step writes the output from the challenge-response process into the specified
files: keyhandle.dat and userkey.dat.

The results of this process must now be registered with the u2f-server-auth-cli command line tool as follows:

    ./u2f-server-auth-cli -g userkey.dat -k keyhandle.dat username
    
This completes the process for manually registering a key for a user.
