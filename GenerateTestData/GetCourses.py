import ChatGemini

model = ChatGemini.model()
course_prompt = '''
    { "上课老师": "王鹏", "课程名": "大学英语精读", "课程号": "gx50941221", "校公选课类别": null, "课程类别": "学科基础课", "课程性质": "必修", "学分": 3, "学时": 48, "考试日期": "2024-04-20", "考试类型": "考察", "显示等级成绩类型": "百分制", "开课单位": "91-公共外语教育学院"}
    参照上面的示例数据， 为我生成50个课程，以JSON输出。每行一个课程数据，中间无需有空格，我会自行格式化。
    其中每个课程对象有如下属性：
1）"上课老师"：必须为中文名字，老师的名字可以为三个字或两个字。名字尽量符合现代取名习惯。
2）"课程名"：必须为中文，不可与校公选课类别相同。课程名应涵盖绝大部分专业。课程名应包含基础课程、专业课程、通识课程。课程名应是中文。通识课程应简洁明了，专业课程应专业性强。
3）"课程号"：这是一个十个字符的字符串，前两个字符必须为字母，后八个字符主要为数字。前两个字母主要是ae，也可以是gx
4）"校公选课类别"：非必须，若该课程有此属性，则在如下数组中取值：// 校公选课类别
const char *pubCourseCategory[] = { "限选大学生心理健康", "哲学智慧与品判思维(I)", "大学生职业发展与就业创业指导", "文化理解与历史传承(II)", "经济与社会发展类(III)", "当代中国与公民责任(III)", "管理与行为科学类(IV)", "全球视野与文明交流(IV)", "艺术鉴赏与审美体验(V)", "工程人文卓越课程类(VII)", "科学精神与创新创造(VI)", "科学与技术类(V)", "生态环境与生命关怀(VII)", "人际沟通与合作精神(VIII)", "文学与艺术类(I)", "创新与创业类(VI)", "历史与文化类(II)", "医学人文卓越课程类(VII)", "慕课"};
5）"课程类别"：在{"通识教育课", "学科基础课", "专业教育课", "拓展课"}中取值
6）"课程性质"：取值范围{"必修", "选修", "限选"}
7）"学分"：1~5，精确到0.5
8）"学时"：整数
9）"考试日期"：YYYY-MM-dd
10）"考试类型"：取值范围{"考试", "考察"}
11）"开课单位":在下面的Institutes数组中取值，具体学院名称已在下面列出，请尽量选取不同的学院。务必与引号内的名称保持一致，如
  "21-计算机科学与技术学院",     // 21
请务必写成
 "学院": "21-计算机科学与技术学院",
不能写成"计算机科学与技术学院"或"计算机学院"。
const char *Institutes[] = {
    "",                            // 0号位置的占位符
    "01-哲学社会学院",             // 1
    "02-文学院",                   // 2
    "03-外国语学院",               // 3
    "04-艺术学院",                 // 4
    "05-体育学院",                 // 5
    "06-经济学院",                 // 6
    "07-法学院",                   // 7
    "08-行政学院",                 // 8
    "09-商学与管理学院B",          // 9
    "10-数学学院",                 // 10
    "11-物理学院",                 // 11
    "12-化学学院",                 // 12
    "13-生命科学学院",             // 13
    "14-机械与航空航天工程学院",   // 14
    "15-汽车工程学院",             // 15
    "16-材料科学与工程学院",       // 16
    "17-交通学院",                 // 17
    "18-生物与农业工程学院",       // 18
    "19-电子科学与工程学院",       // 19
    "20-通信工程学院",             // 20
    "21-计算机科学与技术学院",     // 21
    "22-地球科学学院",             // 22
    "23-地球探测科学与技术学院",   // 23
    "24-建设工程学院",             // 24
    "25-新能源与环境学院",         // 25
    "26-金融学院",                 // 26
    "27-公共卫生学院",             // 27
    "28-药学院",                   // 28
    "29-护理学院",                 // 29
    "30-东北亚学院",               // 30
    "",                            // 31号位置的占位符
    "",                            // 32号位置的占位符
    "33-口腔医学院",               // 33
    "34-商学与管理学院A",          // 34
    "35-商学与管理学院",           // 35
    "36-公共外交学院",             // 36
    "37-新闻与传播学院",           // 37
    "38-考古学院",                 // 38
    "39-马克思主义学院",           // 39
    "40-人工智能学院",             // 40
    "",                            // 41号位置的占位符
    "",                            // 42号位置的占位符
    "",                            // 43号位置的占位符
    "",                            // 44号位置的占位符
    "",                            // 45号位置的占位符
    "",                            // 46号位置的占位符
    "",                            // 47号位置的占位符
    "",                            // 48号位置的占位符
    "",                            // 49号位置的占位符
    "50-预科教育学院",             // 50
    "",                            // 51号位置的占位符
    "",                            // 52号位置的占位符
    "",                            // 53号位置的占位符
    "54-应用技术学院",             // 54
    "55-软件学院",                 // 55
    "",                            // 56号位置的占位符
    "",                            // 57号位置的占位符
    "",                            // 58号位置的占位符
    "",                            // 59号位置的占位符
    "60-国际语言学院",             // 60
    "",                            // 61号位置的占位符
    "",                            // 62号位置的占位符
    "",                            // 63号位置的占位符
    "",                            // 64号位置的占位符
    "65-仪器科学与电气工程学院",   // 65
    "",                            // 66号位置的占位符
    "",                            // 67号位置的占位符
    "",                            // 68号位置的占位符
    "",                            // 69号位置的占位符
    "70-临床医学院",               // 70
    "71-基础医学院",               // 71
    "",                            // 72号位置的占位符
    "",                            // 73号位置的占位符
    "",                            // 74号位置的占位符
    "75-白求恩第一临床医学院",     // 75
    "76-白求恩第二临床医学院",     // 76
    "77-白求恩第三临床医学院",     // 77
    "",                            // 78号位置的占位符
    "79-白求恩医学部",             // 79
    "",                            // 80号位置的占位符
    "81-动物医学学院",             // 81
    "82-植物科学学院",             // 82
    "83-军需科技学院",             // 83
    "84-农学部公共教学中心",       // 84
    "85-动物科学学院",             // 85
    "",                            // 86号位置的占位符
    "87-食品科学与工程学院",       // 87
    "xz27-学生心理健康指导中心",   // 88
    "J1-军事理论教研室",           // 89
    "90-创新创业教育学院",         // 90
    "91-公共外语教育学院",         // 91
    "92-公共体育教学与研究中心",   // 92
    "93-公共数学教学与研究中心",   // 93
    "94-公共物理教学与研究中心",   // 94
    "95-公共化学教学与研究中心",   // 95
    "96-公共计算机教学与研究中心", // 96
    "97-机械基础教学与研究中心",   // 97
    "98-艺术教学与研究中心",       // 98
    "99-团委",                     // 99号位置的占位符
};

'''

model.beginChat(course_prompt, "./GetTestData/__Courses.json")