use anyhow::Result;
use itertools::Itertools;
use std::collections::HashMap;
use std::collections::HashSet;

use std::path::PathBuf;

fn parse_to_sizes(lines: &[String]) -> HashMap<PathBuf, i64> {
    let command_matcher = regex::Regex::new(r"(?P<cmd>\$ cd|\$ ls)\s*(?P<args>.*)$").unwrap();
    let listing_matcher =
        regex::Regex::new(r"dir\s*(?P<dir_name>..*)|(?P<size>\d*)\s\s*(?P<file_name>..*)").unwrap();
    let mut sizes: HashMap<PathBuf, i64> = HashMap::new();
    let mut cwd = PathBuf::new();
    lines.iter().for_each(|line| {
        println!("{}", line);
        if let Some(c) = command_matcher.captures(line) {
            match c.name("cmd").unwrap().as_str() {
                "$ ls" => println!("LIST"),
                "$ cd" => {
                    println!("CHANGE DIR to {}", c.name("args").unwrap().as_str());
                    if ".." == c.name("args").unwrap().as_str() {
                        cwd.pop();
                    } else {
                        let arg = &c.name("args").unwrap().as_str();
                        if arg.starts_with('/') {
                            cwd = PathBuf::from(arg);
                        } else {
                            cwd.push(PathBuf::from(arg));
                        }
                    }
                    println!("new cwd={:?}", cwd);
                }
                _ => todo!(),
            }
            // println!("c={:?}", c);
        } else {
            let l = listing_matcher.captures(line).unwrap();
            println!("l={:?}", l);
            if l.name("size").is_some() {
                let mut file_path = cwd.clone();
                file_path.push(l.name("file_name").unwrap().as_str());
                sizes.insert(
                    file_path,
                    l.name("size").unwrap().as_str().parse::<i64>().unwrap(),
                );
            }
        }
    });

    println!("sizes={:#?}", sizes);
    sizes
}

fn sizes_to_dir_sizes(sizes: &HashMap<PathBuf, i64>) -> HashMap<PathBuf, i64> {
    let mut dir_sizes: HashMap<PathBuf, i64> = HashMap::new();
    let dirs: HashSet<PathBuf> = sizes
        .iter()
        .map(|(path, _)| path)
        .cloned()
        .flat_map(|mut path| {
            path.pop();
            path.ancestors()
                .into_iter()
                .map(|p| p.to_path_buf())
                .collect::<Vec<PathBuf>>()
        })
        .collect();
    println!("dirs: {:?}", &dirs);

    dirs.iter().for_each(|d| {
        let s = sizes
            .iter()
            .filter(|(p, _s)| p.starts_with(d))
            .map(|(_k, v)| v)
            .sum();
        dir_sizes.insert(d.clone(), s);
    });
    println!("dir_sizes={:#?}", dir_sizes);
    dir_sizes
}

pub fn solve_problem_1(lines: &[String]) -> Result<i64> {
    let sizes = parse_to_sizes(lines);
    let dir_sizes = sizes_to_dir_sizes(&sizes);

    Ok(dir_sizes
        .iter()
        .filter(|(_k, v)| **v < 100_000)
        .map(|(_k, v)| v)
        .sum())
}

pub fn solve_problem_2(lines: &[String]) -> Result<i64> {
    let sizes = parse_to_sizes(lines);
    let dir_sizes = sizes_to_dir_sizes(&sizes);

    let in_use: i64 = sizes.iter().map(|(_d, s)| s).sum();
    const TOTAL_SPACE: i64 = 70_000_000;
    const REQUIRED_SPACE: i64 = 30_000_000;

    let free_space = TOTAL_SPACE - in_use;
    let delete_space = REQUIRED_SPACE - free_space;
    println!("free_space={} need={}", free_space, delete_space);

    Ok(*dir_sizes
        .iter()
        .map(|(_d, s)| s)
        .sorted()
        .find(|size| **size > delete_space)
        .unwrap())
}
