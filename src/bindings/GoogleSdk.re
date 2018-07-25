[@bs.scope "gapi"] [@bs.val]
external _load : (string, unit => unit) => unit = "load";

[@bs.deriving abstract]
type auth2InitOptions = {client_id: string};

type googleAuth;

[@bs.scope ("gapi", "auth2")] [@bs.val]
external _auth2Init : auth2InitOptions => Js.Promise.t(googleAuth) = "init";

[@bs.send] [@bs.scope "isSignedIn"]
external signInListener : (googleAuth, bool => unit) => unit = "listen";

[@bs.send]
external _signIn : googleAuth => Js.Promise.t(Js.Json.t) = "signIn";

let decodeSignInResult = json =>
  Json.Decode.(json |> at(["Zi", "id_token"], string));

let signIn = googleAuth =>
  Js.Promise.(
    googleAuth
    |> _signIn
    |> then_(json => resolve(json |> decodeSignInResult))
  );

let googleAuthPromise =
  Js.Promise.(
    make((~resolve, ~reject as _) => {
      _load("auth2", () => {
        _auth2Init(
          auth2InitOptions(
            ~client_id=
              "272502268584-54djmi8t8oj4d3mvlvm5gnma91rrufps.apps.googleusercontent.com",
          ),
        )
        |> then_(ga => {
             resolve(. ga);
             Js.Promise.resolve();
           })
        |> ignore;
        ();
      });
      ();
    })
  );
