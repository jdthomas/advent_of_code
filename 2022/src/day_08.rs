use anyhow::Result;
use itertools::Itertools;
use std::collections::HashMap;
use std::collections::HashSet;

#[derive(Default, Debug)]
struct Tree {
    height: u8,
    visible: bool,
    max_n: u8,
    max_e: u8,
    max_s: u8,
    max_w: u8,
    vs: i64,
}

pub fn solve_problem_1(lines: &[String]) -> Result<i64> {
    // Dumb solution, just scan each direction and build the max so far
    let mut parsed: Vec<Vec<Tree>> = lines
        .iter()
        .map(|line| {
            line.chars()
                .map(|tree| Tree {
                    height: tree as u8 - '0' as u8,
                    ..Default::default()
                })
                .collect::<Vec<Tree>>()
        })
        .collect();

    let w = parsed.len();
    let h = parsed[0].len();
    println!("Your grid is {} x {}", w,h);
    // Do a pass to update the edges (FIXME: only needs to visit edge nodes)
    // w edges:
    (0..w).for_each(|x| {
        let y = 0;
        // All edges are visible
        parsed[x][y].visible = true;
        // Initialize edge maxes
        parsed[x][y].max_w = parsed[x][y].height;
        parsed[x][y].max_n = parsed[x][y].height;
        // println!("init: x={}, y={} ({:?})", x, y, parsed[x][y]);

        let y = h-1;
        parsed[x][y].visible = true;
        parsed[x][y].max_e = parsed[x][y].height;
        parsed[x][y].max_s = parsed[x][y].height;
        // println!("init: x={}, y={} ({:?})", x, y, parsed[x][y]);

    });
    // h edges:
    (0..h).for_each(|y| {
        let x = 0;
        // All edges are visible
        parsed[x][y].visible = true;
        // Initialize edge maxes
        parsed[x][y].max_w = parsed[x][y].height;
        parsed[x][y].max_n = parsed[x][y].height;
        // println!("init: x={}, y={} ({:?})", x, y, parsed[x][y]);

        let x = w-1;
        parsed[x][y].visible = true;
        parsed[x][y].max_e = parsed[x][y].height;
        parsed[x][y].max_s = parsed[x][y].height;
        // println!("init: x={}, y={} ({:?})", x, y, parsed[x][y]);
    });
    
    // Forward pass to collect north / west max heights
    (0..w).cartesian_product(0..h).for_each(|(x, y)| {
        if x > 0 {
        parsed[x][y].max_n = std::cmp::max(parsed[x - 1][y].max_n, parsed[x][y].height);
        }
        if y > 0 {
        parsed[x][y].max_w = std::cmp::max(parsed[x][y - 1].max_w, parsed[x][y].height);
        }
        // println!("fwd: x={}, y={} ({:?})", x, y, parsed[x][y]);
    });

    // Reverse pass to collect east / south max heights
    (0..w).cartesian_product(0..h).collect::<Vec<(usize,usize)>>().into_iter().rev().for_each(|(x, y)| {
        
        if x < w-1 {
        parsed[x][y].max_s = std::cmp::max(parsed[x + 1][y].max_s, parsed[x][y].height);
        }
        if y < h-1 {
        parsed[x][y].max_e = std::cmp::max(parsed[x][y + 1].max_e, parsed[x][y].height);
        }
        // println!("rev: x={}, y={} ({:?})", x, y, parsed[x][y]);
    });

    // Pass of the interior to collect the visibiltiy 
    (1..w-1).cartesian_product(1..h-1).for_each(|(x,y)| {
        parsed[x][y].visible = parsed[x][y].visible
        || (&[
            parsed[x - 1][y].max_n,
            parsed[x + 1][y].max_s,
            parsed[x][y - 1].max_w,
            parsed[x][y + 1].max_e,
        ])
            .iter()
            .min()
            .unwrap()
            < &parsed[x][y].height;
    });

    parsed.iter().for_each(|p| {
        println!("{}", p.iter().map(|p| format!("{}{}: n{} e{} s{} w{}", p.height,if p.visible { "*" } else  {" "}, p.max_n, p.max_e, p.max_s, p.max_w)).join(" | "));
    });

    parsed.iter().for_each(|p| {
        println!("{}", p.iter().map(|t| if t.visible {'X'} else {' '}).join(""));
    });
    

    Ok(parsed
        .iter()
        .flat_map(|trees| trees.iter().map(|tree| tree.visible))
        .filter(|tv| *tv)
        .count() as i64)
}

fn v_score(grid: &[Vec<Tree>], x:usize, y:usize, w:usize, h:usize) -> i64 {
    let mut scores = (0,0,0,0);

    for xx in (0..x).rev() {
        scores.0 += 1;
        if grid[xx][y].height >= grid[x][y].height {
            break
        }
    }
    for xx in x+1..w {
        scores.1 += 1;
        if grid[xx][y].height >= grid[x][y].height {
            break
        }
    }
    for yy in (0..y).rev() {
        scores.2 += 1;
        if grid[x][yy].height >= grid[x][y].height {
            break
        }
    }
    for yy in y+1..h {
        scores.3 += 1;
        if grid[x][yy].height >= grid[x][y].height {
            break
        }
    }
    // println!("{:?} = {} at {} {} ({})", scores, scores.0 * scores.1 * scores.2 * scores.3, x,y, grid[x][y].height);
    scores.0 * scores.1 * scores.2 * scores.3
}
pub fn solve_problem_2(lines: &[String]) -> Result<i64> {
    let mut parsed: Vec<Vec<Tree>> = lines
    .iter()
    .map(|line| {
        line.chars()
            .map(|tree| Tree {
                height: tree as u8 - '0' as u8,
                ..Default::default()
            })
            .collect::<Vec<Tree>>()
    })
    .collect();

    let w = parsed.len();
    let h = parsed[0].len();
    println!("Your grid is {} x {}", w,h);

    (0..w).cartesian_product(0..h).for_each(|(x,y)| {
        parsed[x][y].vs = v_score(&parsed, x,y,w,h);
    });
    // parsed.iter().for_each(|p| {
    //     println!("{}", p.iter().map(|p| format!("{}{}: {}", p.height,if p.visible { "*" } else  {" "}, p.vs)).join(" | "));
    // });
    Ok(parsed.iter().flat_map(|trees| trees.iter().map(|tree| tree.vs)).max().unwrap_or_default())

}
