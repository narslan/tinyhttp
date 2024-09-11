
#include "http.hpp"
#include <view.html.hpp>

static std::vector<std::string> messages;

int main(int argc, char const* argv[])
{
  HttpServer s;

  messages.push_back("Test message");
  messages.push_back("Test message 2 ");
  messages.push_back("Test message 3");
  s.when("/")->serveFile("/home/nevroz/go/src/git.sr.ht/dezge/dist/index.html");
  s.whenMatching("/dist/[^/]+")->serveFromFolder("/home/nevroz/go/src/git.sr.ht/dezge/dist");
  s.whenMatching("/utils/[^/]+")->serveFromFolder("/home/nevroz/go/src/git.sr.ht/dezge/utils");
  s.whenMatching("/elements/[^/]+")
      ->serveFromFolder("/home/nevroz/go/src/git.sr.ht/dezge/elements");
  s.whenMatching("/src/[^/]+")->serveFromFolder("/home/nevroz/go/src/git.sr.ht/dezge/src");
  s.whenMatching("/node_modules/xel/[^/]+")
      ->serveFromFolder("/home/nevroz/go/src/git.sr.ht/dezge/node_modules/xel");
  s.whenMatching("/node_modules/xel/themes/[^/]+")
      ->serveFromFolder("/home/nevroz/go/src/git.sr.ht/dezge/node_modules/xel/themes");

  s.whenMatching("/node_modules/xel/icons/[^/]+")
      ->serveFromFolder("/home/nevroz/go/src/git.sr.ht/dezge/node_modules/xel/icons");
  s.whenMatching("/node_modules/dompurify/dist/[^/]+")
      ->serveFromFolder("/home/nevroz/go/src/git.sr.ht/dezge/node_modules/dompurify/dist");

  s.whenMatching("/classes/[^/]+")
      ->serveFromFolder("/home/nevroz/go/src/git.sr.ht/dezge/node_modules/classes");

  s.when("/messages")
      ->posted([](const HttpRequest& req) {
        auto msg = req.json().toObject();
        messages.push_back(msg["message"].toString());
        return HttpResponse{201};
      })
      ->requested([](const HttpRequest& req) {
        miniJson::Json::_object res;
        res["messages"] = messages;
        return HttpResponse{200, res};
      });

  s.when("/messages/static")->requested([](const HttpRequest&) -> HttpResponse {
    return {200, templates::ViewTemplate(messages)};
  });

  s.when("/shutdown")->requested([&s](const HttpRequest&) -> HttpResponse {
    s.shutdown();
    return {200, "Goodbye!"};
  });

  s.startListening(8088);
  return 0;
}
