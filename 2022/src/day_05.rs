use anyhow::Result;
use itertools::Itertools;
use regex::Regex;

// from: https://stackoverflow.com/questions/64498617/how-to-transpose-a-vector-of-vectors-in-rust
fn transpose<T>(v: Vec<Vec<T>>) -> Vec<Vec<T>> {
    assert!(!v.is_empty());
    let len = v[0].len();
    let mut iters: Vec<_> = v.into_iter().map(|n| n.into_iter()).collect();
    (0..len)
        .map(|_| {
            iters
                .iter_mut()
                .map(|n| n.next().unwrap())
                .collect::<Vec<T>>()
        })
        .collect()
}

fn parse_initial(lines: &[String]) -> Vec<Vec<char>> {
    let mut transposed_result: Vec<Vec<Option<char>>> = lines
        .iter()
        .filter_map(|line| -> Option<Vec<Option<char>>> {
            if line.contains('[') {
                let v = line
                    .chars()
                    .chunks(4)
                    .into_iter()
                    .map(|chunk| {
                        chunk
                            .into_iter()
                            .nth(1)
                            .and_then(|c| if c == ' ' { None } else { Some(c) })
                    })
                    .collect::<Vec<Option<char>>>();
                Some(v)
            } else {
                None
            }
        })
        .collect();
    let maxlen = transposed_result.iter().map(|v| v.len()).max().unwrap();
    transposed_result
        .iter_mut()
        .for_each(|v| v.resize(maxlen, None));

    println!(
        "transposed_result: {:?}",
        transposed_result
            .iter()
            .map(|v| v.len())
            .collect::<Vec<usize>>()
    );
    println!("transposed_result: {:?}", transposed_result);
    transpose(transposed_result)
        .iter()
        .map(|c| c.iter().filter_map(|x| x.as_ref()).rev().cloned().collect())
        .collect()
}

pub fn solve_problem_1(lines: &[String]) -> Result<i64> {
    let mut state = parse_initial(lines);
    println!("Initial state: {:?}", state);
    // todo!();

    let operation_re = Regex::new(r"move (?P<ct>\d*) from (?P<src>\d*) to (?P<dst>\d*)").unwrap();
    lines.iter().for_each(|line| {
        if let Some(op) = operation_re.captures(line) {
            println!("State: {:?}", state);
            println!("Cap: {:?}", op);
            let ct = op.name("ct").unwrap().as_str().parse::<usize>().unwrap();
            let src = op.name("src").unwrap().as_str().parse::<usize>().unwrap() - 1;
            let dst = op.name("dst").unwrap().as_str().parse::<usize>().unwrap() - 1;

            println!("src len={}", state[src].len());

            let new_len = state[src].len() - ct;
            let move_these = state[src].split_off(new_len);
            state[dst].extend(move_these.iter().rev());
        }
    });
    println!("final_state: {:?}", state);
    let soln = state.iter().map(|v| v.last().unwrap_or(&' ')).join("");
    println!("answer: {:?}", soln);

    Ok(42)
}

pub fn solve_problem_2(lines: &[String]) -> Result<i64> {
    let mut state = parse_initial(lines);
    println!("Initial state: {:?}", state);
    // todo!();

    let operation_re = Regex::new(r"move (?P<ct>\d*) from (?P<src>\d*) to (?P<dst>\d*)").unwrap();
    lines.iter().for_each(|line| {
        if let Some(op) = operation_re.captures(line) {
            println!("State: {:?}", state);
            println!("Cap: {:?}", op);
            let ct = op.name("ct").unwrap().as_str().parse::<usize>().unwrap();
            let src = op.name("src").unwrap().as_str().parse::<usize>().unwrap() - 1;
            let dst = op.name("dst").unwrap().as_str().parse::<usize>().unwrap() - 1;

            println!("src len={}", state[src].len());

            let new_len = state[src].len() - ct;
            let move_these = state[src].split_off(new_len);
            state[dst].extend(move_these); // The only change from part1 is not reversing here
        }
    });
    println!("final_state: {:?}", state);
    let soln = state.iter().map(|v| v.last().unwrap_or(&' ')).join("");
    println!("answer: {:?}", soln);

    Ok(42)
}
