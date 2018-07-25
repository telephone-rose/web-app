type authResponse = {
  accessToken: string,
  userID: string,
  expiresIn: int,
  signedRequest: string,
};

type loginStatus =
  | Connected(authResponse)
  | NotAuthorized
  | Unknown;

let decodeAuthResponse = authResponse =>
  Json.Decode.{
    accessToken: authResponse |> field("accessToken", string),
    userID: authResponse |> field("userID", string),
    expiresIn: authResponse |> field("expiresIn", int),
    signedRequest: authResponse |> field("signedRequest", string),
  };

let parseAuthResponse = result =>
  switch (result |> Json.Decode.field("status", Json.Decode.string)) {
  | "connected" =>
    Connected(
      result |> Json.Decode.field("authResponse", decodeAuthResponse),
    )
  | "not_authorized" => NotAuthorized
  | _ => Unknown
  };

[@bs.deriving abstract]
type initOptions = {
  appId: string,
  version: string,
};

[@bs.scope "FB"] [@bs.val] external _init : initOptions => unit = "init";

let init = (~appId) => _init(initOptions(~appId, ~version="v3.0"));

[@bs.deriving abstract]
type loginOptions = {scope: string};

[@bs.scope "FB"] [@bs.val]
external _login : (Js.Json.t => unit, loginOptions) => unit = "login";

let login = (~scopes=[||], callback) =>
  _login(
    result => result |> parseAuthResponse |> callback,
    loginOptions(~scope=Js.Array.join(scopes)),
  );

[@bs.scope "FB"] [@bs.val]
external _getLoginStatus : (Js.Json.t => unit) => unit = "getLoginStatus";

let getLoginStatus = callback =>
  _getLoginStatus(result => {
    result |> parseAuthResponse |> callback;
    ();
  });
