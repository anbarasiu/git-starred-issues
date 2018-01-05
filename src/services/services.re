let env = "";

let getStarredRepos = () : Js.Promise.t(Bs_fetch.response) => 
    env === "DEV" ? 
    Bs_fetch.fetch("repos.json") :
    Bs_fetch.fetch("https://api.github.com/users/anbarasiu/starred");
    
let getIssuesForRepo = (url: string) : Js.Promise.t(Bs_fetch.response) => 
    env === "DEV" ? 
    Bs_fetch.fetch("issues.json") :
    Bs_fetch.fetch(url ++ "/issues");
    
let getLabelsForRepo = (url: string) : Js.Promise.t(Bs_fetch.response) => 
    env === "DEV" ? 
    Bs_fetch.fetch("labels.json") :
    Bs_fetch.fetch(url ++ "/labels");