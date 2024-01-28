#include <includes.hpp>
#include "xorstr.hpp"

#pragma comment(lib, "libcurl.lib")

#define CURL_STATICLIB 

struct channel_struct
{
	std::string author;
	std::string message;
	std::string timestamp;
};

namespace KeyAuth {
	class api {
	public:

		std::string name, ownerid, secret, version, url;

		api(std::string name, std::string ownerid, std::string secret, std::string version, std::string url) : name(name), ownerid(ownerid), secret(secret), version(version), url(url) {}

		void ban(std::string reason = "");
		void init();
		void check();
		void log(std::string msg);
		void license(std::string key);
		std::string var(std::string varid);
		std::string webhook(std::string id, std::string params, std::string body = "", std::string contenttype = "");
		void setvar(std::string var, std::string vardata);
		std::string getvar(std::string var);
		bool checkblack();
		void web_login();
		void button(std::string value);
		void upgrade(std::string username, std::string key);
		void login(std::string username, std::string password);
		std::vector<unsigned char> download(std::string fileid);
		void regstr(std::string username, std::string password, std::string key, std::string email = "");
		void chatget(std::string channel);
		bool chatsend(std::string message, std::string channel);
		void changeusername(std::string newusername);
		std::string fetchonline();
		void fetchstats();
		void forgot(std::string username, std::string email);

		class subscriptions_class {
		public:
			std::string name;
			std::string expiry;
		};

		class data_class {
		public:
			// app data
			std::string numUsers;
			std::string numOnlineUsers;
			std::string numKeys;
			std::string version;
			std::string customerPanelLink;
			// user data
			std::string username;
			std::string ip;
			std::string hwid;
			std::string createdate;
			std::string lastlogin;

			std::vector<subscriptions_class> subscriptions;

			// response data
			std::vector<channel_struct> channeldata;
			bool success{};
			std::string message;
		};
		data_class data;
	private:
		std::string sessionid, enckey;

		static std::string req(std::string data, std::string url);

		void load_user_data(nlohmann::json data) {
			api::data.username = data[XorStr("username")];
			api::data.ip = data[XorStr("ip")];
			if (data[XorStr("hwid")].is_null()) {
				api::data.hwid = XorStr("none");
			}
			else {
				api::data.hwid = data[XorStr("hwid")];
			}
			api::data.createdate = data[XorStr("createdate")];
			api::data.lastlogin = data[XorStr("lastlogin")];

			for (int i = 0; i < data[XorStr("subscriptions")].size(); i++) {
				subscriptions_class subscriptions;
				subscriptions.name = data[XorStr("subscriptions")][i][XorStr("subscription")];
				subscriptions.expiry = data[XorStr("subscriptions")][i][XorStr("expiry")];
				api::data.subscriptions.emplace_back(subscriptions);
			}
		}

		void load_app_data(nlohmann::json data) {
			api::data.numUsers = data[XorStr("numUsers")];
			api::data.numOnlineUsers = data[XorStr("numOnlineUsers")];
			api::data.numKeys = data[XorStr("numKeys")];
			api::data.version = data[XorStr("version")];
			api::data.customerPanelLink = data[XorStr("customerPanelLink")];
		}

		void load_response_data(nlohmann::json data) {
			api::data.success = data[XorStr("success")];
			api::data.message = data[XorStr("message")];
		}

		void load_channel_data(nlohmann::json data) {
			api::data.success = data[XorStr("success")];
			api::data.message = data[XorStr("message")];
			for (auto sub : data[XorStr("messages").c_str()])
			{
				std::string authoroutput = sub[XorStr(("author"))];
				std::string messageoutput = sub[XorStr(("message"))];
				std::string timestampoutput = sub[XorStr(("timestamp"))];
				authoroutput.erase(remove(authoroutput.begin(), authoroutput.end(), '"'), authoroutput.end());
				messageoutput.erase(remove(messageoutput.begin(), messageoutput.end(), '"'), messageoutput.end());
				timestampoutput.erase(remove(timestampoutput.begin(), timestampoutput.end(), '"'), timestampoutput.end());
				channel_struct output = { authoroutput , messageoutput, timestampoutput };
				api::data.channeldata.push_back(output);
			}
		}

		nlohmann::json response_decoder;

	};
}
