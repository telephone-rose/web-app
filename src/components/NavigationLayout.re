let component = ReasonReact.statelessComponent("NotFound");

let rootStyle =
  Glamor.(css([flexDirection("column"), display("flex"), flex("1")]));

let make = children => {
  ...component,
  render: _self =>
    <div className=rootStyle>
      <div className="tabs">
        <ul>
          <li className="is-active">
            <a> ("Discover" |> ReasonReact.string) </a>
          </li>
          <li> <a> ("Messages" |> ReasonReact.string) </a> </li>
          <li> <a> ({j|Paramètres|j} |> ReasonReact.string) </a> </li>
        </ul>
      </div>
      children
    </div>,
};
