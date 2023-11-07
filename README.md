# ircserv

An IRC server that is capable of holding many (up to 1000) IRC clients.

- Downloading

```sh
git clone git@github.com:AkewakBiru/ircserv.git
```

- Installation

```sh
make
```

- Running the program

```sh
./ircserv PORT PASSWORD
```

### Simple connection with an nc client

```sh
echo -n "user user * * :user\r\nnick nick\r\npass SERVER_PASSWORD\r\n" > connect_me
```

```sh
cat connect_me - | nc SERVER_IP SERVER_PORT
```

### Sending command with nc client

```sh
command - should end with CRLF -> to replicate CR (carrage-return), use CTRL + V, then CTRL + M and for LF (line-feed), use return key.
```

### It also works well with irssi (an official IRC client)
