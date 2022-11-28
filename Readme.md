### This Project purpose is to create a simple IPC within the GNC by using UPD as communication mode

#### Requirements 

IPC, One process can send a UDP message to another process.

- Client Sends Gray Scale Images at 2592 x 1444
- Server Revcieves the above image and queues image
- Images are queued in CicularBuffer depth N
- Images are processed from above buffer and forwared on.
- It is unclear if the is Request/Responce needed

#### Dependencies

- boost::asio
- boost

```
cmake .. -G"Visual Studio 16 2019" -DCMAKE_TOOLCHAIN_FILE=C:/Users/e438262/dev/github/vcpkg/scripts/buildsystems/vcpkg.cmake
```

### Regarding Size

The image alone
```
2591 * 1444 = 3,742, 848 bytes = 3.742848 MB = 0.003742848G
```



### Synchronous UDP Client 

- Acts as a client in the client-server communication model
- Communicates with the server application using UDP
- Uses I/O that block the thread
    - execution until the corresponding operation completes
    - or an error occurs

### Functionaly

- Given IP-address(s) and port(s) number 
- Allocate a UDP socket.
- Exchange messages with the servers.
- Deallocate the socket.