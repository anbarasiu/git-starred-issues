let styles = Css.({
  "listItem": style([
    listStyleType(None),
    textAlign(Left),
    margin(px(8)),
    border(px(1), Solid, lightgray),
    borderRadius(px(10)),
    padding(px(4))
  ]),
  "issues": style([
    fontStyle(Oblique),
    color(darkred)
  ]),
  "title": style([
    fontWeight(Bold)
  ]),
  "link": style([
    wordWrap(BreakWord)
  ])
});

let component = ReasonReact.statelessComponent("ListItem");

let make = (~name, ~url, ~issues=?, _children) => {
  {
  ...component,
  render: (_self) => {
    let issuesCount = switch(issues) {
    | None => ""
    | Some(issues) => "Issues: " ++ string_of_int(issues)
    };
    <li className=styles##listItem>
      <div className=styles##title>(ReasonReact.stringToElement(name))</div>
      <a className=styles##link href=url>(ReasonReact.stringToElement(url))</a>
      <div className=styles##issues>(ReasonReact.stringToElement({issuesCount}))</div>
    </li>
    }
  }
};