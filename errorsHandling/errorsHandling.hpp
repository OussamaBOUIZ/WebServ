#ifndef __ERRORSHANDLING_H__
# define __ERRORSHANDLING_H__

# include "../Interfaces/Client.hpp"

class ClientInfo;
bool    isTransferEncodingNotChunked(std::map<std::string, std::string> &requestData);
bool    isNotValidPostRequest(std::map<std::string, std::string> &requestData);
bool    isBodySizeBigger(ServerConfiguration &serverConfig, unsigned int bodySize);
const char *get_real_format(const char *mime_type);
const char *get_mime_format(const char *type);
void		error_414(ClientInfo *);
void		error_501(ClientInfo *);
void		error_400(ClientInfo *);
void		error_500(ClientInfo *);
void		error_413(ClientInfo *);
void		error_404(ClientInfo *);
#endif