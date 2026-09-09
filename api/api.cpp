#include <iostream>
#include <string>
#include <sstream>

#include "httplib.h"

#include "Parser1.hpp"
#include "Scanner1.hpp"

#include "parserService.hpp"

using namespace std;
using namespace calc;


int main()
{
    httplib::Server svr;


    // -------------------------
    // CORS
    // -------------------------

    svr.set_pre_routing_handler(
        [](const httplib::Request& req,
           httplib::Response& res)
        {

            res.set_header(
                "Access-Control-Allow-Origin",
                "http://localhost:4200"
            );

            res.set_header(
                "Access-Control-Allow-Methods",
                "GET, POST, OPTIONS"
            );

            res.set_header(
                "Access-Control-Allow-Headers",
                "Content-Type, Accept"
            );


            if (req.method == "OPTIONS") {

                res.status = 200;

                return httplib::Server::HandlerResponse::Handled;
            }


            return httplib::Server::HandlerResponse::Unhandled;
        }
    );


    // -------------------------
    // POST /comando
    // -------------------------

    svr.Post(
        "/comando",
        [](const httplib::Request& req,
           httplib::Response& res)
        {

            cout << "-----------------------------" << endl;

            cout << "Comando recibido:" << endl;

            cout << req.body << endl;


            string resultado = ejecutarParser(
                req.body
            );


            cout << "Resultado:" << endl;

            cout << resultado << endl;

            cout << "-----------------------------" << endl;


            res.set_content(
                resultado,
                "text/plain"
            );
        }
    );


    cout << "Servidor iniciado en puerto 8080..."
         << endl;


    svr.listen(
        "0.0.0.0",
        8080
    );
}