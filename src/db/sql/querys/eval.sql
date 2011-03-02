-- this is query file for project evaluator
-- query names starting with _ are of internal use

:matija_spava:1::
  select 2,3,4,5,?;

:_get_tables:::
  select name, mod-- nest oneos nesssto
  from system.tables; 

:matija_2:3::
  select * 
    from task where id = ? 
    and ? = ?;

:matija_3::tasks:
  delete from tasks;

--
--	
--	Ovo je sql file koji sadrži sve query-e koji se koriste
--	u rasporedu. Php skripta ce ga ucitat i parsat funkcijom
--
--	baza::parseSqlTemplate();
--
--	Syntaxa je naravno SQL osim imena query-a, koji izgleda::
--		:([a-zA-Z0-9_]):([0-9]):([.*])
--	a sadrzi ime query-a u prvoj zagradi, broj varijabli koje
--	zahtjeva u drugoj zagradi, te poslije toga sam query
--	prazne linije su dozvoljene
--  testiram hooook od doma!

:get_tb_modified:0::
	select tablename, date_part('epoch', modified) from tb;

:update_tb_modified:2::
	update tb 
		set modified = to_timestamp('?')::timestamp without time zone
		where tablename = ?;
		
:login_valid:4::
	select userid from "user" where 
		pass = ? and 
		((userid = ?) or 
		(email = ?) or (nickname = ?));

:user_get_by_nickname:1::
  select userid from "user" where nickname = ?;

:user_get_imeprezime:2::
	select userid from "user" where ime = ? and prezime = ?;

:user_get_by_key:1::
	select userid from "user" where ext_key = ?;

:user_get:1::
	select 
    "user".*,
    skola.naziv, skola.grad, skola.nivo,
    razred.popunjen, razred.skolaid,
    razred.odjel, razred.turnus, razred.matura
	from "user"
	left join razred on razred.razredid = "user".razredid
	left join skola on skola.skolaid = razred.skolaid
	where userid = ?

:user_get_attr:1::
	select userid, "user".razredid, razred.skolaid, tip
		from "user" left join razred on "user".razredid = razred.razredid
		where userid = ?;

:user_choose:2::
	select userid from "user"
	where user_match(?,cita) and
		(coalesce(ime,'') || ' ' || 
		coalesce(prezime,'') || ' ' || 
		coalesce(nickname,'') || ' ' ||
		coalesce(userid::text ,'') || ' ' ||
		coalesce(email,'') || ' ' ||
    coalesce(aai,'')) ~* ?
	limit 10;

:user_get_by_cita:1::
	select userid from "user" where user_match(userid,?);

:skole_update_list:3::
	insert into skola (naziv,grad,nivo) values(?,?,?);

:skola_get_naziv:1::
	select * from skola where to_tsvector(naziv) @@ plainto_tsquery(?);

:user_generate_choose:2::
	select userid, ime, prezime, email, nickname from "user"
	where user_match(?,cita) and position(? in ime) ;
	

:skole_update_one:4:skola(4):
	update skola set naziv=?,grad=?,nivo=? where skolaid=?;

:skole_daj_listu:2::
	select skolaid, naziv, grad, nivo from skola where grad = ? and nivo = ?;

:skole_daj_listu:1::
	select skolaid, naziv, grad, nivo from skola where grad = ?;

:skola_valid:2::
	select skolaid, naziv, grad, nivo from skola where grad = ? and skolaid = ?;

:grad_valid:1::
	select grad from skola where grad ~* ? order by length(grad) limit 1;

:skole_get_all:0::
	select * from skola;
	
:raspored_sati_get:4::
	select 
		rasporedid, active_od, active_do, raspored.predavanjeid, raspored.skgod,
		dvorana.*,
		termin.*,
		pred_alias, profesorid, predmet.naziv
	
	from raspored
	
	left join dvorana
		on raspored.dvoranaid = dvorana.dvoranaid
		
	join termin
		on raspored.terminid = termin.terminid
				
	join predavanje
		on raspored.predavanjeid = predavanje.predavanjeid
	
	join sudjeluje
		on raspored.predavanjeid = sudjeluje.predavanjeid and 
			sudjeluje.userid = ?
	
	join predmet
		on predavanje.predmetid = predmet.predmetid
		
	where ima_presjek(active_od,active_do,?,?) and predavanje.skgod = ?;

:raspored_get_r:5::
	select rasporedid, predavanje.predavanjeid, termin.dan, termin.sat, termin.ciklus, active_od, active_do
	from raspored
	join termin on raspored.terminid = termin.terminid
	join predavanje on raspored.predavanjeid = predavanje.predavanjeid
	where predavanje.razredid = ? and predavanje.skgod = ? and termin.sat = ? 
		and ima_presjek(active_od,active_do,?,?);

:raspored_get_rupa:4::
	select rasporedid, predavanje.predavanjeid, termin.dan, termin.sat, termin.ciklus, active_od, active_do
	from raspored
	join termin on raspored.terminid = termin.terminid
	join predavanje on raspored.predavanjeid = predavanje.predavanjeid
	where predavanje.razredid = ? and predavanje.skgod = ?
		and ima_presjek(active_od,active_do,?,?);

:raspored_get_debug:2::
	select rasporedid, termin.dan, termin.sat, termin.ciklus, active_od, active_do, naziv
	from raspored
	join termin on raspored.terminid = termin.terminid
	join predavanje on raspored.predavanjeid = predavanje.predavanjeid
	join predmet on predavanje.predmetid = predmet.predmetid
	where predavanje.razredid = ? and predavanje.skgod = ?
	order by rasporedid desc;

:raspored_get:1::
	select 
		rasporedid, raspored.terminid, termin.ciklus, dvoranaid, 
		raspored.predavanjeid, predavanje.razredid, predavanje.predmetid,
		termin.dan, active_od, active_do, predavanje.skgod
	from raspored
	join termin on raspored.terminid = termin.terminid
	join predavanje on raspored.predavanjeid = predavanje.predavanjeid
	where rasporedid = ?;

:raspored_del:1:raspored(1):
	delete from raspored where rasporedid = ?;

:dvorana_insert:1::
	insert into dvorana (kratki) values(?);

:dvorana_get:1::
	select dvoranaid from dvorana where kratki = ?;

:raspored_put:5::
	insert into raspored 
		(predavanjeid,terminid,dvoranaid,active_od,active_do)
	values (?,?,?,?,?);

:raspored_update:5:raspored(5):
	update raspored set terminid = ?, dvoranaid = ?, active_od = ?, active_do = ?
	where rasporedid = ?;

:termin_get:3::
	select terminid from termin where dan = ? and sat = ? and ciklus = ? and vrijeme is null;

:termin_insert:3::
	insert into termin (dan,sat,ciklus) values(?,?,?);

:zvona_get:1::
	select vrijeme, sat, trajanje from zvono where shemaid = ? order by sat;

:shema_postojiNaziv:2::
	select shemaid from shema where naziv = ? and skolaid = ?;

:shema_postojiNaziv:1::
	select shemaid from shema where naziv = ?;

:shema_fill:4::
	insert into zvono (shemaid,vrijeme,sat,trajanje) values(?,?,?,?);

:shema_update:5:zvono:
	update zvono set vrijeme = ?, sat = ?, trajanje = ? where sat = ? and shemaid = ?;

:shema_get:1: 
	select shemaid, owner, opis, shema.skolaid, shema.naziv, skola.naziv as skola_naziv, grad
	from shema left join skola on skola.skolaid = shema.shemaid
	where shemaid = ?;
	
:shema_list_sk:2::
	select shemaid, owner, opis, skolaid, naziv, cita, pise from shema 
	where skolaid = ? or (skolaid is not null and user_match(?,cita)) order by shemaid;

:shema_list_global:0::
	select shemaid, owner, opis, skolaid, naziv, cita, pise from shema
	where skolaid is null order by shemaid;

:shema_list_sati:0::
	select 
		shema.shemaid, min(zvono.sat) as prvi,
		max (zvono.sat) as zadnji, min(zvono.vrijeme) as pocetak
	from shema
	left join zvono on zvono.shemaid = shema.shemaid group by (shema.shemaid);

:shema_delete:1:shema(1):
	delete from shema where shemaid = ?;

:shema_insert:6::
	insert into shema (skolaid, owner,opis,naziv,cita,pise) values(?,?,?,?,?,?);
	
:predmet_get_all:0::
	select predmetid, naziv, smjer from predmet order by not (smjer ~* '^osnovna' or smjer ~* '^Srednja -'), smjer, predmetid;
	
:predavanje_r_get:2::
	select
		predavanje.predmetid, predavanje.profesorid, pred_alias, skgod,
		predavanje.predavanjeid, naziv, smjer, ime, prezime
	from predavanje
	join predmet on predmet.predmetid = predavanje.predmetid
	left join profesor on profesor.profesorid = predavanje.profesorid
	where predavanje.razredid = ? and skgod = ? order by pred_alias;

:predavanje_get:1::
	select
		predavanje.predmetid, predavanje.profesorid, pred_alias, skgod,
		predavanje.predavanjeid, naziv, smjer, ime, prezime
	from predavanje
	join predmet on predmet.predmetid = predavanje.predmetid
	left join profesor on profesor.profesorid = predavanje.profesorid
	where predavanje.predavanjeid = ?
	order by predmet.naziv;

:predavanje_get:3::
	select
		predavanje.predmetid, predavanje.profesorid, pred_alias, skgod,
		predavanje.predavanjeid, naziv, smjer, ime, prezime
	from predavanje
	join predmet on predmet.predmetid = predavanje.predmetid
	left join profesor on profesor.profesorid = predavanje.profesorid
	where predavanje.razredid = ? and predavanje.predmetid = ? and skgod = ?;

:predavanje_get_skolska:2::
	select
		predavanje.predmetid, predavanje.profesorid, pred_alias, skgod,
		predavanje.predavanjeid,
		predavanje.razredid, naziv, smjer, ime, prezime, odjel, matura
	from predavanje
	join predmet on predmet.predmetid = predavanje.predmetid
	join razred on razred.razredid = predavanje.razredid
	left join profesor on profesor.profesorid = predavanje.profesorid
	where razred.skolaid = ? and skgod = ?
	order by matura, odjel, pred_alias;

:predavanje_update:3:predavanje(3):
	update predavanje set pred_alias = ?, profesorid = ?
	where predavanjeid = ?;
	
:predavanje_fill:7::
	insert into predavanje 
		(predmetid,razredid,profesorid,pred_alias,skgod,cita,pise)
	values(?,?,?,?,?,?,?);

:profesor_exists:2::
	select profesorid from profesor where ime = ? and prezime = ?;

:profesor_insert:2::
	insert into profesor (ime,prezime) values(?,?);
	
:is_sudjeluje:2::
	select predavanjeid from sudjeluje
	where userid = ? and predavanjeid = ?;
	
:sudjeluje_get:2::
	select predavanje.razredid, sudjeluje.predavanjeid, predavanje.predmetid, naziv, smjer, ime, prezime, pred_alias, predavanje.skgod
	from sudjeluje
	join predavanje on 
		predavanje.predavanjeid = sudjeluje.predavanjeid
	left join profesor on profesor.profesorid = predavanje.profesorid
	join predmet on predmet.predmetid = predavanje.predmetid
	where userid = ? and predavanje.skgod = ?;

:sudjeluje_empty:2:sudjeluje:
	delete from sudjeluje where userid = ? and 
		predavanjeid in (select predavanjeid
			from predavanje 
			where predavanje.skgod = ?);

:sudjeluje_empty_r:2:sudjeluje:
	delete from sudjeluje predavanjeid in 
		(select predavanjeid
			from predavanje 
			where razredid = ? and predavanje.skgod = ?);

:event_delay:4:event(4):
	update event set rasporedid = ?, datum = ?, predavanjeid = ?
	where eventid = ?;

:event_detach_pred:3:spajalica,komentar,event:
	delete from spajalica where objectid in
		(select eventid from event where predavanjeid = ?);
	delete from komentar where objektid in
		(select eventid from event where predavanjeid = ?);
	delete from event where predavanjeid = ?;
	
:sudjeluje_empty_pred:1:sudjeluje:
	delete from sudjeluje where predavanjeid = ?;

:raspored_empty_pred:1:raspored:
	delete from raspored where predavanjeid = ?;

:sudjeluje_insert:2::
	insert into sudjeluje 
		(userid,predavanjeid) values(?,?);

:sudjeluje_insert:5::
	izazovi gresku
	insert into sudjeluje 
		(userid,predmetid,razredid,skgod,predavanjeid) values(?,?,?,?,?);

:sudjeluje_insert_svim_r:2::
	insert into sudjeluje (userid,predavanjeid)
	(select userid, ? from "user" where razredid = ?)
	
:sudjeluje_insert_sva_r:3::
	insert into sudjeluje (userid,predavanjeid)
	(select ?, predavanjeid from predavanje where razredid = ? and skgod = ?);

:last_login:1::
	select vrijeme from log
	where action = 'login' and objectid = ?
	order by vrijeme desc;

:log_insert:7::
	insert into log (userid,msg,objectid,loglevel,ip,details,action)
		values (?,?,?,?,?,?,?);

:log_list:12::
	select userid,msg,objectid,loglevel,vrijeme,ip,details,action
	from log 
  where
    (? = 1 or userid = ?) and
    (? = 1 or msg = ?) and
    (? = 1 or ip = ?) and
    (? = 1 or loglevel = ?) and
    (? = 1 or action = ?) 
  order by vrijeme desc limit ? offset ?;

:log_list:2::
	select userid,msg,objectid,loglevel,vrijeme,ip,details,action
	from log where vrijeme > ? and vrijeme < ? order by vrijeme desc;

:log_list_user:2::
	select userid,msg,objectid,loglevel,vrijeme,ip,details,action
	from log where userid = ? order by vrijeme desc limit ?;

:log_feed:2::
	select userid,msg,objectid,vrijeme,action
	from log
	where action is not null and not (action = 'upgrade') and not (action = 'login') and not (action = 'aai_login') and not (action = 'logout')
	and vrijeme > ? and vrijeme < ? order by vrijeme desc limit 500;

:log_feed_object:2::
	select userid,msg,objectid,vrijeme,action,details
	from log
	where action is not null and not (action = 'upgrade') and not (action = 'login') and not (action = 'aai_login') and not (action = 'logout')
	and (objectid = ? or msg = ?) order by vrijeme desc;
	
:period_list:1::
	select periodid,naziv,shemaid,boja,active_od,active_do,dani_mask,razredid,ima_nastave
	from period
	where razredid = ? order by active_od desc;
	
:period_insert:8::
	insert into period 
		(naziv,shemaid,boja,active_od,active_do,dani_mask,razredid,ima_nastave)
	values(?,?,?,?,?,?,?,?);
	
:period_update:9:period(9):
	update period
		set naziv = ?, shemaid = ?, boja = ?, active_od = ?, active_do = ?,
		dani_mask = ?, razredid = ?, ima_nastave = ?
	where periodid = ?;
	
:period_del:1:period(1):
	delete from period where periodid = ?;
	
:period_get:3::
	select periodid,naziv,shemaid,boja,active_od,active_do,dani_mask,razredid,ima_nastave
	from period
	where razredid = ? and ima_presjek(active_od,active_do+1,?,?)
	order by prioritet desc, periodid desc;

:action_postoji:1::
	select eventid from event where actionid = ?;

:event_action_delete:3:spajalica,komentar,event:
	delete from spajalica where objectid in
		(select eventid from event where actionid = ?);
	delete from komentar where objektid in
		(select eventid from event where actionid = ?);
	delete from event where actionid = ?;
	
:action_list_global:0::
	select actionid,naziv,boja,razredid,cita,pise,prioritet
	from action where razredid is null order by prioritet desc;

:action_list_my:1::
	select actionid,naziv,boja,razredid,cita,pise,prioritet
	from action where razredid is not null and user_match(?,cita) order by prioritet desc;
	
:action_insert:6::
	insert into action (naziv,boja,razredid,cita,pise,prioritet)
		values (?,?,?,?,?,?);

:action_update:7:action(7):
	update action set naziv=?, boja=?, razredid=?, cita=?, pise=?,prioritet=?
	where actionid = ?;
	
:event_insert:8::
	insert into event
		(rasporedid,predavanjeid,datum,actionid,info,owner,cita,pise)
		values(?,?,?,?,?,?,?,?);
		
:event_update:3:event(3):
	update event set actionid = ?, info = ? where eventid = ?;

:event_get_tjedan:3::
	select eventid, action.actionid, action.naziv, action.boja, action.prioritet,
		rasporedid, datum, info, owner
	from event
	join action on event.actionid = action.actionid
	join sudjeluje
		on event.predavanjeid = sudjeluje.predavanjeid and 
			sudjeluje.userid = ?
	where datum >= ? and datum <= ?
	order by action.prioritet desc;

:event_get_tjedan:4::
	select eventid, action.actionid, action.naziv, action.boja, action.prioritet,
		rasporedid, datum, info, owner
	from event
	join action on event.actionid = action.actionid
	join sudjeluje
		on event.predavanjeid = sudjeluje.predavanjeid and 
			sudjeluje.userid = ?
	where datum >= ? and datum <= ?
	order by eventid desc limit ?;

:event_get_multi:3::
	select eventid, event.predavanjeid
	from event
	join predavanje
		on event.predavanjeid = predavanje.predavanjeid
	join sudjeluje
		on event.predavanjeid = sudjeluje.predavanjeid and 
			sudjeluje.userid = ?
	where datum = ? and rasporedid = ?;

:event_get_otpali:2::
	select eventid, event.predavanjeid
	from event
	join predavanje
		on event.predavanjeid = predavanje.predavanjeid
	join sudjeluje
		on event.predavanjeid = sudjeluje.predavanjeid and 
			sudjeluje.userid = ?
	where datum = ? and rasporedid is null;

:komentar_koliko:2::
	select count(komentarid) as koliko from komentar
	where objektid = ? and owner = ?;

:komentar_del_e:1:komentar:
	delete from komentar where objektid = ?;

:komentar_insert:5::
	insert into komentar (objektid,owner,komentar,cita,pise) values(?,?,?,?,?);

:komentar_get_owners:1::
	select owner from komentar where
	objektid = ? order by komentarid desc;

:komentar_get_parent:1::
	select objektid from komentar where komentarid = ?;
	
:komentar_get_last:1::
	select komentarid, owner, komentar, cita, pise from komentar where
	objektid = ? order by komentarid desc limit 1;

:komentar_get:1::
	select komentarid, owner, komentar, cita, pise from komentar where
	objektid = ? order by komentarid desc;

:users_get_skole:0::
	select distinct(razred.skolaid), skola.naziv, skola.grad from razred
	join skola on razred.skolaid = skola.skolaid;
	
:razredi_get:1::
	select razredid,skolaid,matura,odjel,turnus from razred where skolaid = ?;
	
:razred_popuni:1:razred(1):
  update razred set popunjen = 1 where razredid = ?;

:razred_postoji:3::
	select razredid from razred
	where skolaid = ? and matura = ? and lower(odjel) = lower(?);
	
:razred_insert:3::
	insert into razred (skolaid,matura,odjel) values(?,?,lower(?));

:users_get_razredni:1::
	select userid,ime,prezime,email,tip,last_seen,cita,pise from "user"
	where razredid = ? order by tip desc, prezime;

:token_delete_hash:1:token:
	delete from token where hash = ?;

:token_make:4::
	insert into token (hash,cita,parms,tip) values (?,?,?,?);
	
:token_get:1::
	select tokenid,hash,cita,parms,expire,tip from token
	where hash = ?;

:token_clean:0:token:
	delete from token where expire < now()::date;

:users_insert:8::
	insert into "user" (razredid,ime,prezime,pass,aai,tip,email,nickname)
		values(?,?,?,?,?,?,?,?);
		
:users_insert:9::
	insert into "user" (razredid,ime,prezime,pass,aai,tip,email,nickname,fb_uid)
		values(?,?,?,?,?,?,?,?,?);
		
:users_insert:11::
	insert into "user" 
      (razredid,ime,prezime,
      pass,tip,email,nickname,aai_uid,fb_uid,google_uid,enabled)
		values(?,?,?,?,?,?,?,?,?,?,?);

:users_update_perms:3:user(3):
	update "user" set cita = ?, pise = ? where userid = ?;
	
:razred_get_radmin:1::
	select userid from "user" where razredid = ? and tip = 5 order by prezime desc;

:razred_get_num:1::
	select count(userid) as br from "user" where razredid = ?;

:user_update_last_seen:1:user(1):
	update "user" set last_seen = now() where userid = ?;

:user_update_ime:3:user(3):
	update "user" set ime = ?, prezime = ? where userid = ?;

:user_update_tip:2:user(2):
	update "user" set tip = ? where userid = ?;
	
:user_enable:1:user(1):
	update "user" set enabled = 1 where userid = ?;

:user_prereject:1:user(1):
	update "user" set enabled = -1 where userid = ?;

:user_update_email:2:user(2):
	update "user" set email = ? where userid = ?;

:user_update_pass:2:user(2):
	update "user" set pass = ? where userid = ?;
	
:user_update_avatar:2:user(2):
	update "user" set avatar = ? where userid = ?;

:user_update_conf:2:user(2):
	update "user" set conf = ? where userid = ?;
				
:user_update_fb_uid:2:user(2):
	update "user" set fb_uid = ? where userid = ?;
	
:user_update_aai_uid:2:user(2):
	update "user" set aai_uid = ? where userid = ?;

:user_update_google_uid:2:user(2):
	update "user" set google_uid = ? where userid = ?;

:event_detach:1:event:
	update event set rasporedid = null where
	rasporedid = ?;

:event_migrate:4:event:
	update event set rasporedid = ? where 
	rasporedid = ? and ima_presjek(datum,datum+1,?,?);

:skripta_where:1:
	select objectid from spajalica where skriptaid = ?;

:skripta_rename:2:skripta(2):
	update skripta set filename = ? where skriptaid = ?;

:skripta_byhash:1::
	select skriptaid, cita, pise from skripta where filehash = ?;

:skripta_hits_inc_object:1:skripta:
	update skripta set hits = hits + 1 
		where skriptaid in 
			(select skriptaid from spajalica where objectid = ?);

:skripta_update_stupanj:2:skripta(2):
	update skripta set stupanj = ?
		where skriptaid = ?;

:skripta_update_pred:3:skripta(3):
	update skripta set predavanjeid = ?, predmetid = ?
		where skriptaid = ?;

:skripta_anulate_predavanjeid:1:skripta:
	update skripta set predavanjeid = null where predavanjeid = ?;

:skripta_update_predavanjeid:3:skripta(3):
	update skripta
		set predavanjeid = ?, predmetid = 
			(select predmetid from predavanje where predavanjeid = ?)
	where skriptaid = ?;

:skripta_hits_inc:1:skripta(1):
	update skripta set hits = hits + 1 where skriptaid = ?;

:skripta_update_hits:2:skripta(2):
	update skripta set hits = ? where skriptaid = ?;

:skripta_spojena:1::
	select spajalica.objectid from spajalica where skriptaid = ?;

:skripta_pre_upload:9::
	insert into skripta (filename,filesize,filehash,cita,pise,uploader,stipid,predmetid,stupanj)
	values (?,?,?,?,?,?,?,?,?);
	
:skripta_get_event:1::
	select spajalica.skriptaid from spajalica 
	join skripta on skripta.skriptaid = spajalica.skriptaid
	where objectid = ? order by filesize desc;

:skripta_na_eventu:2::
	select skriptaid from spajalica where skriptaid = ? and objectid = ?;

:skripta_ima:2::
	select spajalica.skriptaid, filesize from spajalica
	join skripta on skripta.skriptaid = spajalica.skriptaid
	where objectid = ? and user_match( ?, cita ) order by filesize desc;

:skripta_zakvaci:2::
	insert into spajalica (skriptaid, objectid) values (?,?);
	
:skripta_detach_event:1:spajalica:
	delete from spajalica where objectid = ?;

:skripta_update_stats:3:skripta(3):
	update skripta set filesize = ?, filehash = ? where skriptaid = ?;

:skripta_update_stip:2:skripta(2):
	update skripta set stipid = ? where skriptaid = ?;

:skripta_rename:2:skripta(2):
	update skripta set filename = ? where skriptaid = ?;

:skripta_detach:1:spajalica:
	delete from spajalica where skriptaid = ?;

:skripta_detach:2:spajalica:
	delete from spajalica where skriptaid = ? and objectid = ?;
	
:skripta_update_perm:3:skripta(3):
	update skripta set cita = ?, pise = ? where skriptaid = ?;

:skripta_list_all:0::
	select skriptaid from skripta;

-- stari novi stari novi
:spajalica_prespoji:4:spajalica:
	delete from spajalica where
		(skriptaid = ? and objectid in 
			(select objectid from spajalica where skriptaid = ?));
	update spajalica set skriptaid = ? where skriptaid = ?;

:komentar_prespoji:2:komentar:
	update komentar set objektid = ? where objektid = ?;
	
--userid, {{razredid, -1}}
:skripta_get_ploha_root:2::
  select skripta.* from skripta
  where 
    user_match(?, cita) and
    user_match(uploader, ?) and
    predmetid is null;

:folder_get_ploha_root:2::
  select distinct predmetid from skripta
    where (predmetid is not null) and
    user_match(?, cita) and
    (user_match(uploader, ?));

:skripta_get_ploha_pred:3::
  select skripta.* from skripta
  where 
    user_match(?, cita) and
    user_match(uploader, ?) and
    predmetid = ? and stipid is null;

:folder_get_ploha_pred:3::
  select distinct stipid, predmetid from skripta
    where (predmetid is not null) and
    user_match(?, cita) and
    (user_match(uploader, ?)) and
    predmetid = ? and stipid is not null;

-- (stupanj enabled 0 ili 1),(int stupanj),
-- (predmet enabled 0 ili 1),(cita maska za slusam|razred|skola)
-- (vlalsnik enabled 0 ili 1),(cita maska za vlasnika)
--array( $est, $st, $epr, $epr, $predmetid, $epr, $predmetid, $epr, $predmetid, $evl, $vl ), true );
:skripta_get_filter:11::
		(select skripta.* from skripta where ? = 0 or stupanj = ?)
	intersect
		(select skripta.* from skripta
		where ? = 0 or (
			(? = 1 and (skripta.predmetid in
					(select predmetid from sudjeluje
						join predavanje on predavanje.predavanjeid = sudjeluje.predavanjeid
						where sudjeluje.userid = ?)
				))
			or (? = 2 and (skripta.predmetid in
					(select predmetid from predavanje where razredid = ?)
				))
			or (? = 3 and (skripta.predmetid in
					(select predmetid from predavanje
						join razred on predavanje.razredid = razred.razredid
						where skolaid = ?)
				))))
	intersect
		(select skripta.* from skripta
			where ? = 0 or (uploader is not null and user_match(uploader,?)));
	
:stip_list:0::
	select * from stip order by stipid desc;

:stip_postoji:1::
	select stipid from stip where naziv = ?;
	
:stip_insert:1::
	insert into stip (naziv) values(?);




:poll_insert:8::
	insert into poll (pitanje,tip,active_do,owner,cita,pise,title,vidljivost)
		values(?,?,?,?,?,?,?,?);

:poll_update:9:poll(9):
	update poll set pitanje = ?, tip = ?, active_do = ?, owner = ?, cita = ?,
		pise = ?, title = ?, vidljivost = ?
		where pollid = ?;
		
:vote_insert:2::
	insert into vote (pollid,odgovor) values(?,?);

:poll_get:1::
	select pollid, pitanje, title, tip, vidljivost, active_do, owner, cita, pise
	from poll where user_match(?,cita) order by pollid desc;

:vote_get:1::
	select vote.pollid, voteid, odgovor, glasovi
		from vote
		join poll on vote.pollid = poll.pollid
		where user_match(?,poll.cita)
		order by glasovi desc;

:vote_get_one_poll:1::
	select voteid, odgovor, glasovi
		from vote where pollid = ?
		order by glasovi desc;

:tko_cita:1::
	select userid from "user" where user_match( userid, ? );

:is_netko_glasated:1::
	select vote.pollid from log
	join vote on vote.voteid = log.objectid
	where loglevel = 5 and action = 'vote' and pollid = ? limit 1;

:is_glasated:2: 
	select vrijeme, vote.* from log
	join vote on vote.voteid = log.objectid
	where loglevel = 5 and action = 'vote' and pollid = ? and userid = ?;
	
:vote:2:vote(2):
	update vote set glasovi = glasovi + ? where voteid = ?;
	
:poll_reset:2:vote,log:
	update vote set glasovi = 0 where pollid = ?;
	delete from log where objectid in
		(select voteid from vote where pollid = ?);
		
:poll_delete:2:vote,poll(2):
	delete from vote where pollid = ?;
	delete from poll where pollid = ?;
	



:users_get_sys_admin:0::
	select userid from "user" where tip = 6;
	
:poruka_insert:3:poruka:
	insert into poruka (salje,cita,tema) values(?,?,?);

:poruka_get_sve:1::
	select porukaid, max(komentar.komentarid) as kid from poruka
	left join komentar on komentar.objektid = poruka.porukaid
	where user_match( ?, poruka.cita )
	group by porukaid order by kid desc;
	
:poruka_get_sent:2::
	select porukaid, max(komentar.komentarid) as kid from poruka
	left join komentar on komentar.objektid = poruka.porukaid
	where owner = ? and user_match( ?, poruka.cita )
	group by porukaid order by kid desc;

:poruka_get_inbox:2::
	select porukaid, max(komentar.komentarid) as kid from poruka 
	left join komentar on komentar.objektid = poruka.porukaid
	where user_match( ?, poruka.cita ) and not komentar.owner = ?
	group by porukaid order by kid desc;
	
:has_seen:2::
	select userid, objektid from user_vidio_objekt
	where userid = ? and objektid = ?;

:set_seen:2::
	insert into user_vidio_objekt (userid,objektid) values (?,?);

:objekt_fade:1::
	delete from user_vidio_objekt where objektid = ?;
	
:user_blind:1::
	delete from user_vidio_objekt where userid = ?;
	
:objekt_get_unseen:1::
	select objekt.objektid as objektid, tablename from objekt
	left join user_vidio_objekt on
		user_vidio_objekt.objektid = objekt.objektid and
		user_vidio_objekt.userid = ?
	where
		tablename in ('poll','poruka','skripta','event') and
		user_vidio_objekt.objektid is null and
		(created > now() - interval '7 day' or tablename = 'poruka' or tablename = 'anketa')

:user_delete:2:sudjeluje,user(2):
  delete from sudjeluje where userid = ?;
  delete from "user" where userid = ?;

:get_last_users:0::
  select * from "user" order by userid desc limit 50;

:user_get_by_fb_uid:1::
  select userid,enabled from "user" where fb_uid = ?;

:user_get_by_google_uid:1::
  select userid,enabled from "user" where google_uid = ?;

:user_get_by_aai_uid:1::
  select userid,enabled from "user" where aai_uid = ?;

