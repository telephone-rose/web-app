let makeTitle = (~emojiResume) =>
  switch (emojiResume) {
  | Some(emojiResume) when String.length(emojiResume) > 0 =>
    Some({j|“|j} ++ emojiResume ++ {j|”|j})
  | _ => None
  };

let string_of_distance = distance =>
  switch (distance) {
  | 1 => {j|1 mètre"|j}
  | distance when distance < 1000 => {j|$(distance) mètres|j}
  | distance =>
    string_of_float(Utils.round(float_of_int(distance) /. 1000.0, 1))
    ++ {j| kilomètres|j}
  };

let makeSubtitle = (~userFirstName, ~userCity, ~distance) =>
  switch (userCity, distance) {
  | (Some(userCity), Some(distance)) =>
    {j|- $(userFirstName) de $(userCity) à |j}
    ++ string_of_distance(distance)
  | (None, Some(distance)) =>
    {j|- $(userFirstName) à |j} ++ string_of_distance(distance)
  | (Some(city), None) => {j|- $(userFirstName) de $(city)|j}
  | (None, None) => {j|- $(userFirstName)|j}
  };

let component = ReasonReact.statelessComponent("DiscoverCard");

let cardStyle =
  Glamor.(
    css([
      marginTop("24px"),
      marginBottom("24px"),
      marginLeft("24px"),
      marginRight("24px"),
      borderRadius("12px"),
    ])
  );

let make =
    (
      ~distance,
      ~emojiResume,
      ~userCity,
      ~userFirstName,
      ~userId as _: string,
      ~recordingDownloadUrl,
      ~onNopeClick,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div className={"card " ++ cardStyle}>
      <div className="card-content">
        {
          switch (makeTitle(~emojiResume)) {
          | None => ReasonReact.null
          | Some(emojiResume) =>
            <p className="title"> {emojiResume |> ReasonReact.string} </p>
          }
        }
        {
          switch (recordingDownloadUrl) {
          | None => ReasonReact.null
          | Some(downloadUrl) =>
            <div>
              <audio controls=true>
                <source src=downloadUrl datatype="audio/mpeg" />
              </audio>
            </div>
          }
        }
        <p className="subtitle">
          {
            makeSubtitle(~userFirstName, ~userCity, ~distance)
            |> ReasonReact.string
          }
        </p>
      </div>
      <footer className="card-footer">
        <a onClick=onNopeClick className="card-footer-item has-text-danger">
          {"Nope" |> ReasonReact.string}
        </a>
        <a href="#" className="card-footer-item has-text-success">
          {{j|Répondre|j} |> ReasonReact.string}
        </a>
      </footer>
    </div>,
};
