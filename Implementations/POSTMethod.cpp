# include "../Interfaces/POSTMethod.hpp"
# include "../errorsHandling/errorsHandling.hpp"

PostMethod::PostMethod(ServerConfiguration &serverConfiguration)
: _serverConfiguration(serverConfiguration), totalTempFileSize(0), toWrite(0)
{}


//void    PostMethod::_searchForCurrentLocation(ClientInfo *client) {
//    std::list<LocationBlockParse>::iterator beg = this->_serverConfiguration.Locations.begin();
//    for (; beg != this->_serverConfiguration.Locations.end(); beg++)
//    {
//        LocationBlockParse loc = *beg;
//        std::string res = loc.Location;
//        std::string path =  client->parsedRequest.requestDataMap["path"];
//        int len = path.length() - 1;
//        int index_last = len;
//        if (path[len] == '/')
//            len--;
//        bool is_file_last = 0, point = 0;
//        for (; len >= 0; len--) {
//            if (path[len] == '.')
//                point = 1;
//            if (path[len] == '/' && point) {
//                is_file_last = 1;
//                index_last = len;
//                break;
//            } else if (path[len] == '/' && !point) break;
//        }
//        if (res[res.length() - 1] != '/') res += '/';
//        std::string full_path = path.substr(0, index_last + 1);
//        if (!is_file_last && full_path[full_path.length() - 1] != '/') full_path += '/';
//        if (full_path != res) continue;
//        client->_currentLocation = beg;
////        std::list<std::pair<std::string, std::string> >::iterator CGIit = this->.CGI.begin();
////        for(std::list<> ; CGIit != client->_currentLocation.CGI.end(); CGIit++ ){
////                if(!strcmp(CGIit->first.c_str(), cgi_format) && !strcmp(cgi_format, "php")){
////                    client->cgiContentLength =  "0";
////                    client->cgiContentType = "";
////                    client->CGIexecutedFile(currentPath, client, serverConfig, CGIit);
////                    return ;
////                }
////                else if(!strcmp(CGIit->first.c_str(), cgi_format) && !strcmp(cgi_format, "py")){
////                    // python cgi
////                }
////        }
//        return ;
//    }
//    client->_currentLocation = beg;
//}

bool    PostMethod::_isLocationSupportsPost(ClientInfo *client) {
    std::list<std::string>::iterator it = client->_currentLocation->allowedMethods.begin();
    while (it != client->_currentLocation->allowedMethods.end()){
        if (*it == "POST")
            return (true);
        it++;
    }
    return (false);
}

bool    PostMethod::_isLocationSupportsUpload( ClientInfo *client ) {
    return (client->_currentLocation->UploadDirectoryPath.length());
}
void    PostMethod::handleFirstRead(ClientInfo *client) {
     if(client->_currentLocation == this->_serverConfiguration.Locations.end())
     {
         error_404(client);
         throw std::runtime_error("Location not found");
     }
     if (!this->_isLocationSupportsPost(client))
     {
         error_500(client);
         throw (std::runtime_error("Post Method is not supported !!")); // this line was just added and need to be tested.....
     }
    if (isBodySizeBigger(this->_serverConfiguration, client->parsedRequest.contentLength))
     {
//         error_404(client); 413
        throw (std::runtime_error("Body Size Too Large !!"));
    }
    if(this->_isLocationSupportsUpload(client))
     {
         if(client->parsedRequest.isBoundaryExist == true) {
             client->parsedRequest._parsingMiniHeader();
         }
         else {
             client->parsedRequest.uploadFileName = client->generateRandString() + get_real_format(client->parsedRequest.requestDataMap["Content-Type:"].c_str());
         }
         client->postRequest->_preparingPostRequest(client);
         client->postRequest->_isValidPostRequest(client);
     }
     else
     {
     }
}

void PostMethod::_preparingPostRequest(ClientInfo *client) {
    int a = (client->parsedRequest. isBoundaryExist == true) ? 4 : 0;
    int b = (client->parsedRequest.isBoundaryExist == true) ? 0 : 4;
    client->requestBody.open(TMP_FOLDER_PATH + client->parsedRequest.uploadFileName, std::ios::binary);
    client->requestBody.write(client->parsedRequest.requestHeader + client->parsedRequest.bodyIndex + b, client->parsedRequest.received - client->parsedRequest.newBodyIndex - a);
}


void PostMethod::_isValidPostRequest(ClientInfo *client) {
    if(isNotValidPostRequest(client->parsedRequest.requestDataMap))
    {
        error_400(client);
        throw std::runtime_error(BAD_REQUEST_EXCEPTION);
    }
    if(isTransferEncodingNotChunked(client->parsedRequest.requestDataMap))
    {
        error_501(client);
        throw std::runtime_error(TRANSFER_ENCODING_EXCEPTION);
    }
//    std::map<std::string, std::string>::iterator content = client->parsedRequest.requestDataMap.find("Content-Length:");
//    if(content != client->parsedRequest.requestDataMap.end())
//    {
////        std::cout << "the content length is : " << (*_clientInfoIt  )->parsedRequest.contentLength << std::endl;
//        int bodySize = (client->parsedRequest.bodyIndex) - std::stoi(client->parsedRequest.requestDataMap["Content-Length:"]);
//        if(isBodySizeBigger(this->_serverConfiguration, bodySize))
//        {
//            error_413(this->_clientInfoIt);
//            throw std::runtime_error(BODY_SIZE_EXCEPTION);
//        }
//    }
}


void PostMethod::_writeInTempFile(ClientInfo *client) {
    client->requestBody.write(client->parsedRequest.requestHeader, client->parsedRequest.receivedBytes);
    if (client->requestBody.fail())
        throw (std::runtime_error("Error Occurred In writeIntTempFile\n"));
}

void PostMethod::_receiveFromClient(ClientInfo *client){
    client->parsedRequest.bytesToReceive = (client->parsedRequest.received + MAX_REQUEST_SIZE < client->parsedRequest.contentLength) ? MAX_REQUEST_SIZE : client->parsedRequest.contentLength - client->parsedRequest.received;
    if ((client->parsedRequest.receivedBytes = recv(client->socket, client->parsedRequest.requestHeader, client->parsedRequest.bytesToReceive, 0)) == -1)
        throw (std::runtime_error("Error Occurred In ReceiveFromClient\n"));
    client->parsedRequest.received += client->parsedRequest.receivedBytes;
    client->parsedRequest.requestHeader[client->parsedRequest.receivedBytes] = 0;
}

void PostMethod::receiveTheBody(ClientInfo *client){
    this->_receiveFromClient(client);
    this->_writeInTempFile(client);
    if (client->parsedRequest.received == client->parsedRequest.contentLength)
        this->preparingMovingTempFile(client);
};


void PostMethod::preparingMovingTempFile(ClientInfo *client) {
    int i = 0;
    this->totalTempFileSize = client->parsedRequest.received - client->parsedRequest.boundarySize - client->parsedRequest.newBodyIndex;
    if (client->parsedRequest.isBoundaryExist == true)
        totalTempFileSize -= 8; // for skipping /r/n/r/n twice.
    this->toWrite = 0;
    client->requestBody.close();
    struct stat st;
    if (!(stat(client->_currentLocation->UploadDirectoryPath.c_str(), &st) == 0 && S_ISDIR(st.st_mode))) {
        i = mkdir(client->_currentLocation->UploadDirectoryPath.c_str(), O_CREAT | S_IRWXU | S_IRWXU | S_IRWXO);
    }
    this->sourceFile.open(TMP_FOLDER_PATH + client->parsedRequest.uploadFileName, std::ios::binary);
    std::string filePath = client->_currentLocation->UploadDirectoryPath + "/" + client->parsedRequest.uploadFileName;
    this->destinationFile.open(filePath, std::ios::binary);
    if (i == -1 || this->destinationFile.fail())
        throw (std::runtime_error("Error Occurred in preparingMovingTempFile"));
}

void PostMethod::writeToUploadedFile()
{
    this->toWrite = (this->totalTempFileSize > 1024) ? 1024 : this->totalTempFileSize;
    char buffer[this->toWrite + 1];
    this->sourceFile.read(buffer, this->toWrite);
    buffer[this->toWrite] = 0;
    this->destinationFile.write(buffer, this->toWrite);
    if (this->sourceFile.fail() || this->destinationFile.fail())
        throw (std::runtime_error("Error Occurred in writeToUploadedFile"));
    this->totalTempFileSize -= this->toWrite;
}

void  PostMethod::successfulPostRequest(ClientInfo *client){
    this->sourceFile.close();
    this->destinationFile.close();
    std::string path = "uploadSuccess.html";
    int i = remove((TMP_FOLDER_PATH + client->parsedRequest.uploadFileName).c_str());
    if(client->served.is_open())
        client->served.close();
    client->served.open(path);
    client->served.seekg(0, std::ios::end);
    int file_size = client->served.tellg();
    client->served.seekg(0, std::ios::beg);
    std::string error_header = "";
    error_header += "HTTP/1.1 201 Created\r\n"
    + std::string("Connection: close\r\n")
    + std::string("Content-Length: ")
    + std::to_string(file_size)
    + "\r\n"
    +  std::string("Content-Type: ")
    +  get_mime_format(path.c_str())
    + "\r\n\r\n" ;
    if (send(client->socket, error_header.c_str(), error_header.length(), 0) == -1 || i == -1)
        throw (std::runtime_error("Error Occurred in successfulPostRequest"));
}