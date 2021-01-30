ld halfplane_intersection(vector<point> init, vector<line> init_lines, vector<line> lines) {
    for (line l : lines) {
        int cnt = 0;
        for (point p : init) {
            if (l.a * p.x + l.b * p.y + l.c >= 0) {
                cnt++;
            }
        }
        if (cnt == 0) {
            return 0;
        }
        if (cnt == (int)init.size()) {
            continue;
        }
        for (int i = 1; i < (int)init.size(); i++) {
            if (l.a * init[i].x + l.b * init[i].y + l.c >= 0 && l.a * init[i - 1].x + l.b * init[i - 1].y + l.c < 0) {
                rotate(init.begin(), init.begin() + i, init.end());
                rotate(init_lines.begin(), init_lines.begin() + i, init_lines.end());
                break;
            }
        }
        int j = 0;
        while (l.a * init[j].x + l.b * init[j].y + l.c >= 0)
            j++;

        point a = intersect(init_lines[j - 1], l);
        point b = intersect(l, init_lines.back());
        line last_line = init_lines.back();
        init.erase(init.begin() + j, init.end());
        init_lines.erase(init_lines.begin() + j, init_lines.end());
        init.push_back(a); init.push_back(b);
        init_lines.push_back(l); init_lines.push_back(last_line);
    }

    ld sq = 0;
    forn(i, init.size()) {
        sq += cp(init[i], init[(i + 1) % init.size()]);
    }
    return fabsl(sq) / 2;
}
